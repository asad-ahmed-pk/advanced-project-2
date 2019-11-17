//
// FumaroleDetector.cpp
// Takes input for fumarole thermal images and detects fumaroles
//

#include "detection/FumaroleDetector.hpp"
#include "model/FumaroleType.hpp"
#include "config/config.hpp"
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

        // process each image result
        for (const auto& localization : localizations)
        {
            // process each set of contour points for this image result
            for (const auto& contours : localization.second)
            {
                // convert to rect and add to this file's list
                rect = cv::boundingRect(contours);
                detectionResult.ImageID = localization.first;
                detectionResult.BoundingBox = rect;
                detectionResult.Type = type;
                results[localization.first].push_back(detectionResult);
            }
        }

        return std::move(results);
    }

    // Save results as images
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
                cv::rectangle(image, r.BoundingBox, r.Type == Model::FumaroleType::FUMAROLE_OPEN_VENT ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 166, 255));
            }

            path = Config::FINAL_RESULTS_OUTPUT_DIR;
            path += result.first;
            path += Config::IMAGE_OUTPUT_EXT;

            cv::imwrite(path, image);
        }
    }
}
