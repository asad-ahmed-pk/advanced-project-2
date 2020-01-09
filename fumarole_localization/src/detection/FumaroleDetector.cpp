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
#include <algorithm>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Detection
{
    const int FUMAROLE_HOLE_RADIUS { 5 };
    const int DRAW_THICKNESS { 3 };

    // Constructor
    FumaroleDetector::FumaroleDetector(bool saveIntermediateResults) : m_SaveResults(saveIntermediateResults)
    {
        // load params from config file
        m_MinAreaForHeatedArea = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.min_area_heated_area");
        m_OpenVentSearchRadius = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.open_vent_radius_search");
        m_HiddenVentSearchRadius = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.hidden_area_radius_search");
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
        Pipeline::Pipeline pipeline(files, m_SaveResults);

        // run pipeline
        if (pipeline.Run())
        {
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

         // add hidden vents
         std::vector<FumaroleDetection> hiddenVents = std::move(DetectHiddenVents(detections));
         detections.insert(detections.end(), hiddenVents.begin(), hiddenVents.end());

         return std::move(detections);
    }

    // Get classifications for open vents
    std::vector<FumaroleDetection> FumaroleDetector::DetectOpenVents(const std::vector<FumaroleDetection>& detections) const
    {
        // get all detected holes and cluster them into open vents
        std::vector<FumaroleDetection> holes;
        std::copy_if(detections.begin(), detections.end(), std::back_inserter(holes), [](const FumaroleDetection& d){ return d.Type == Model::FumaroleType::FUMAROLE_HOLE; });
        return std::move(ClusterDetections(holes, m_OpenVentSearchRadius, Model::FumaroleType::FUMAROLE_OPEN_VENT));
    }

    std::vector<FumaroleDetection> FumaroleDetector::DetectHiddenVents(const std::vector<FumaroleDetection> &detections) const
    {
        // get all detected heated areas and cluster them into hidden vents
        std::vector<FumaroleDetection> heatedAreas;
        std::copy_if(detections.begin(), detections.end(), std::back_inserter(heatedAreas), [](const FumaroleDetection& d){ return d.Type == Model::FumaroleType::FUMAROLE_HEATED_AREA; });
        return std::move(ClusterDetections(heatedAreas, m_HiddenVentSearchRadius, Model::FumaroleType::FUMAROLE_HIDDEN_VENT));
    }

    // Cluster the given detections based on a radius search
    std::vector<FumaroleDetection> FumaroleDetector::ClusterDetections(const std::vector<FumaroleDetection>& detections, float radius, Model::FumaroleType type) const
    {
        std::vector<FumaroleDetection> clusteredDetections;

        if (detections.empty()) {
            return clusteredDetections;
        }

        // convert to vector of centroids
        std::vector<cv::Point2f> centroids;
        std::transform(detections.begin(), detections.end(), std::back_inserter(centroids), [](const FumaroleDetection& d) -> cv::Point2f {
            return d.Center();
        });

        // get index graph of radius search
        std::map<int, std::vector<int>> matchedIndices;
        RadiusSearch(centroids, matchedIndices, radius);

        // cluster into open vents
        std::vector<bool> used(detections.size(), false);
        std::vector<cv::Rect> boxes;

        for (auto iter = matchedIndices.begin(); iter != matchedIndices.end(); iter++)
        {
            used[iter->first] = true;
            boxes.push_back(detections[iter->first].BoundingBox);

            for (int index : iter->second)
            {
                if (!used[index]) {
                    // get all bounding boxes from the detections with these indices
                    boxes.push_back(detections[index].BoundingBox);
                    used[index] = true;
                }
            }

            if (boxes.size() > 1)
            {
                // build new bounding box that encloses these boxes
                FumaroleDetection detection;
                detection.Type = type;
                detection.BoundingBox = EnclosingBoundingBox(boxes);

                clusteredDetections.push_back(detection);
            }

            boxes.clear();
        }

        return std::move(clusteredDetections);
    }

    // Radius search for each point
    void FumaroleDetector::RadiusSearch(const std::vector<cv::Point2f> &centroids,
                                        std::map<int, std::vector<int>> &matchedIndices, float radius) const
    {
        float d = 0.0;
        for (int i = 0; i < centroids.size(); i++)
        {
            for (int j = 0; j < centroids.size(); j++)
            {
                // skip self distance
                if (i == j) {
                    continue;
                }

                // calculate euclidean distance between this point and centroid
                d = sqrt(pow(centroids[i].x - centroids[j].x, 2) + pow(centroids[i].y - centroids[j].y, 2));
                if (d <= radius) {
                    matchedIndices[i].push_back(j);
                }
            }
        }
    }

    // Get enclosing bounding box that encloses all the given bounding boxes
    cv::Rect FumaroleDetector::EnclosingBoundingBox(const std::vector<cv::Rect> &boxes) const
    {
        auto minXIter = std::min_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return r1.x < r2.x; });
        auto minYIter = std::min_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return r1.y < r2.y; });
        auto maxXIter = std::max_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return (r1.x + r1.width) < (r2.x + r2.width); });
        auto maxYIter = std::max_element(boxes.begin(), boxes.end(), [](const cv::Rect& r1, const cv::Rect& r2){ return (r1.y + r1.height) < (r2.y + r2.height); });

        int x = minXIter->x - FUMAROLE_HOLE_RADIUS;
        int y = minYIter->y - FUMAROLE_HOLE_RADIUS;
        int width = maxXIter->x + maxXIter->width - x + FUMAROLE_HOLE_RADIUS;
        int height = maxYIter->y + maxYIter->height - y + FUMAROLE_HOLE_RADIUS;

        return cv::Rect(x, y, width, height);
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
            //IO::GetThermalImage(result.first, image, true);

            // load full-scale image
            IO::GetFullResCamImage(result.first, image);

            // draw graphics for the different types of vents
            for (const FumaroleDetection& r : result.second)
            {
                if (r.Type == Model::FumaroleType::FUMAROLE_HOLE) {
                    cv::circle(image, r.Center(), FUMAROLE_HOLE_RADIUS, Model::ColorForType(r.Type), DRAW_THICKNESS);
                }
                else {
                    cv::rectangle(image, r.BoundingBox, Model::ColorForType(r.Type), DRAW_THICKNESS);
                }
            }

            path = Config::FINAL_RESULTS_OUTPUT_DIR;
            path += result.first;
            path += Config::IMAGE_OUTPUT_EXT;

            cv::imwrite(path, image);
        }
    }
}
