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
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Detection
{
    // Constructor
    FumaroleDetector::FumaroleDetector()
    {
        // load min area for detection of heated area
        m_MinAreaForHeatedArea = Config::ConfigParser::GetInstance().GetValue<float>("config.detection.min_area_heated_area");
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
        for (const auto& localization : localizations)
        {
            // classify current localizations
            results[localization.first] = std::move(ClassifyLocalizations(localization.second));

            // TODO: add classifications for open vents and hidden vents
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

         return std::move(detections);
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
                return cv::Scalar(56, 230, 25);

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



            // draw all detected bounding boxes on this image
            for (const FumaroleDetection& r : result.second) {
                cv::rectangle(image, r.BoundingBox, ColorForType(r.Type));
            }

            path = Config::FINAL_RESULTS_OUTPUT_DIR;
            path += result.first;
            path += Config::IMAGE_OUTPUT_EXT;

            cv::imwrite(path, image);
        }
    }
}
