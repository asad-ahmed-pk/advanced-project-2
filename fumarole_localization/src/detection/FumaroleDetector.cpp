//
// FumaroleDetector.cpp
// Takes input for fumarole thermal images and detects fumaroles
//

#include "detection/FumaroleDetector.hpp"
#include "model/FumaroleType.hpp"
#include "config/config.hpp"
#include "config/ConfigParser.hpp"
#include "io/fumarole_data_io.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <Eigen/Eigen>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/flann/flann.hpp>

namespace Detection
{
    const int FUMAROLE_HOLE_RADIUS { 5 };

    // Constructor
    FumaroleDetector::FumaroleDetector()
    {
        // load min area for detection of heated area
        m_MinAreaForHeatedArea = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.min_area_heated_area");

        // load min clustering distance to group holes into open vents and heated areas into hidden vents
        m_MinClusterDistance = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.min_distance_for_grouping");
    }

    // Destructor
    FumaroleDetector::~FumaroleDetector()
    {

    }

    // One-shot detection on single thermal image
    bool FumaroleDetector::DetectFumaroles(const std::string& fileID, const std::string &thermalImagePath, std::vector<Detection::FumaroleDetection> &results) const
    {
        // create the input map for the pipeline
        std::map<std::string, std::string> pipelineInput;
        pipelineInput[fileID] = thermalImagePath;

        // use the overloaded function for processing multiple images
        std::map<std::string, std::vector<Detection::FumaroleDetection>> multipleFileResults;
        if (DetectFumaroles(pipelineInput, multipleFileResults))
        {
            results = std::move(multipleFileResults[fileID]);
            return true;
        }

        return false;
    }

    bool FumaroleDetector::DetectFumaroles(const std::map<std::string, std::string> &files, std::map<std::string, std::vector<Detection::FumaroleDetection>> &results) const
    {
        // create a detection pipeline
        Pipeline::Pipeline pipeline(files);

        // run pipeline
        if (pipeline.Run()) {
            // convert localizations of pipelines into results
            results = std::move(ConvertLocalizations(pipeline.GetLocalizations(), Model::FumaroleType::FUMAROLE_OPEN_VENT));
            return true;
        }

        return false;
    }

    // Convert localizations from pipeline into detection results
    std::map<std::string, std::vector<FumaroleDetection>> FumaroleDetector::ConvertLocalizations(const Pipeline::PipelineLocalizations &localizations, Model::FumaroleType type) const
    {
        std::map<std::string, std::vector<FumaroleDetection>> results;
        cv::Rect rect;
        FumaroleDetection detectionResult;

        // process each set of localizations for each image
        for (const auto& localization : localizations) {
            // classify current localizations
            results[localization.first] = std::move(ClassifyLocalizations(localization.second));
        }

        return std::move(results);
    }

    // Classify current localizations (holes and heated areas)
    std::vector<FumaroleDetection> FumaroleDetector::ClassifyLocalizations(const std::vector<std::vector<cv::Point>> &contours) const
    {
         std::vector<FumaroleDetection> detections;

         for (const auto& contour : contours)
         {
             FumaroleDetection detection;

             detection.Type = (cv::contourArea(contour) >= m_MinAreaForHeatedArea ? Model::FumaroleType::FUMAROLE_HEATED_AREA : Model::FumaroleType::FUMAROLE_HOLE);
             detection.BoundingBox = cv::boundingRect(contour);
             detection.Contour = contour;

             detections.emplace_back(std::move(detection));
         }

         // add open vents
         std::vector<FumaroleDetection> openVents = std::move(DetectOpenVents(detections));
         detections.insert(detections.end(), openVents.begin(), openVents.end());

         return std::move(detections);
    }

    // Get classifications for open vents
    std::vector<FumaroleDetection> FumaroleDetector::DetectOpenVents(const std::vector<FumaroleDetection>& detections) const
    {
        // get all detected holes
        std::vector<FumaroleDetection> holeDetections;
        std::copy_if(detections.begin(), detections.end(), std::back_inserter(holeDetections), [](const FumaroleDetection& d){ return d.Type == Model::FumaroleType::FUMAROLE_HOLE; });

        // cluster all holes
        std::vector<std::vector<FumaroleDetection>> clusters = ClusterDetections(holeDetections);

        std::vector<FumaroleDetection> detectedOpenVents;

        // create bounding boxes for clusters that have more than 1 hole (these are open vents)
        std::vector<cv::Rect> boxes;
        for (auto iter = clusters.begin(); iter != clusters.end(); iter++)
        {
            if (iter->size() > 1)
            {
                std::transform(iter->begin(), iter->end(), std::back_inserter(boxes), [](const FumaroleDetection& d) -> cv::Rect { return d.BoundingBox;} );

                FumaroleDetection detection;
                detection.Type = Model::FumaroleType ::FUMAROLE_OPEN_VENT;
                detection.BoundingBox = EnclosingBoundingBox(boxes);

                detectedOpenVents.push_back(detection);

                boxes.clear();

                // TODO: Remove this after done debugging
                break;
            }
        }

        return std::move(detectedOpenVents);
    }

    // Get enclosing bounding box that encloses all the given bounding boxes
    cv::Rect FumaroleDetector::EnclosingBoundingBox(const std::vector<cv::Rect> &boxes) const
    {
        auto minXIter = std::min_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return r1.x < r2.x; });
        auto minYIter = std::min_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return r1.y < r2.y; });
        auto maxXIter = std::max_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return (r1.x + r1.width) < (r2.x + r2.width); });
        auto maxYIter = std::max_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return (r1.y + r1.height) < (r2.y + r2.height); });

        float x = minXIter->x;
        float y = minYIter->y;
        float width = maxXIter->x - x;
        float height = maxYIter->y - y;

        return std::move(cv::Rect(x, y, width, height));
    }

    // Cluster detections into clusters based on l2 distance between centroids of bounding boxes
    std::vector<std::vector<FumaroleDetection>> FumaroleDetector::ClusterDetections(const std::vector<FumaroleDetection> &detections) const
    {
        std::vector<FumaroleDetection> unclusteredDetections = detections;
        std::vector<std::vector<FumaroleDetection>> clusteredDetections;

        std::vector<FumaroleDetection>::iterator iter;

        cv::Rect clusteringBoundingBox;

        while(!unclusteredDetections.empty())
        {
            iter = unclusteredDetections.begin();
            clusteringBoundingBox = std::move(BoundingBoxForCluster(*iter));

            clusteredDetections.emplace_back();
            clusteredDetections[clusteredDetections.size() - 1].push_back(*iter);
            iter = unclusteredDetections.erase(iter);

            // run to end and add all remaining points that belong to this cluster bounding box
            while (iter != unclusteredDetections.end())
            {
                // get centroid of rect and see if inside bounding box
                cv::Point2f center = cv::Point2f(iter->BoundingBox.x + iter->BoundingBox.width / 2.0, iter->BoundingBox.y + iter->BoundingBox.height / 2.0);
                if (clusteringBoundingBox.contains(center)) {
                    clusteredDetections[clusteredDetections.size() - 1].push_back(*iter);
                    iter = unclusteredDetections.erase(iter);
                }
                else {
                    iter++;
                }
            }
        }

        return std::move(clusteredDetections);
    }

    // Get the bounding box that is to be used for clustering
    cv::Rect FumaroleDetector::BoundingBoxForCluster(const Detection::FumaroleDetection &d) const
    {
        cv::Rect b;

        // get centroid of bounding box
        cv::Point2f center = d.Center();

        // create another bounding box with this as center
        b.x = center.x - m_MinClusterDistance;
        if (b.x < 0) b.x = 0;
        b.width = 2 * m_MinClusterDistance;

        b.y = center.y - m_MinClusterDistance;
        if (b.y < 0) b.y = 0;
        b.height = 2 * m_MinClusterDistance;

        cv::Rect b2 = d.BoundingBox;
        return std::move(b2);
    }

    // Get the color for the given type
    cv::Scalar FumaroleDetector::ColorForType(Model::FumaroleType type) const
    {
        switch (type)
        {
            case Model::FumaroleType::FUMAROLE_HOLE:
                return cv::Scalar(10, 245, 206);

            case Model::FumaroleType::FUMAROLE_HEATED_AREA:
                return cv::Scalar(40, 80, 230);

            case Model::FumaroleType::FUMAROLE_OPEN_VENT:
                return cv::Scalar(255, 0, 0);

            case Model::FumaroleType::FUMAROLE_HIDDEN:
                return cv::Scalar(222, 114, 47);
        }
    }

    // Save results as images with colors for different classes
    void FumaroleDetector::SaveResults(const Detection::FumaroleDetectionsPerImage &resultMap) const
    {
        // create output dir if needed
        if (!boost::filesystem::exists(Config::FINAL_RESULTS_OUTPUT_DIR)) {
            boost::filesystem::create_directory(Config::FINAL_RESULTS_OUTPUT_DIR);
        }

        // save all images
        cv::Mat image;
        std::string path;

        for (const auto& result : resultMap)
        {
            // load the original greyscale image
            IO::GetThermalImage(result.first, image, true);

            // draw graphics for the different types of vents
            for (const FumaroleDetection& r : result.second)
            {
                if (r.Type == Model::FumaroleType::FUMAROLE_HOLE) {
                    cv::circle(image, r.Center(), FUMAROLE_HOLE_RADIUS, ColorForType(r.Type));
                }
                else {
                    cv::rectangle(image, r.BoundingBox, ColorForType(r.Type));
                }
            }

            path = Config::FINAL_RESULTS_OUTPUT_DIR;
            path += result.first;
            path += Config::IMAGE_OUTPUT_EXT;

            cv::imwrite(path, image);
        }
    }
}
