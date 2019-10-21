//
// FumaroleRecognizer.cpp
// Takes input for fumarole thermal images and recognizes fumaroles
//

#include "recognition/FumaroleRecognizer.hpp"
#include "model/FumaroleType.hpp"
#include "config/config.hpp"
#include "io/fumarole_data_io.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Recognition
{
    // Constructor
    FumaroleRecognizer::FumaroleRecognizer()
    {


    }

    // Destructor
    FumaroleRecognizer::~FumaroleRecognizer()
    {

    }

    // One-shot detection on single thermal image
    bool FumaroleRecognizer::RecognizeFumaroles(const std::string& fileID, const std::string &thermalImagePath, std::vector<Recognition::FumaroleDetectionResult> &results) const
    {
        // create the input map for the pipeline
        std::map<std::string, std::string> pipelineInput;
        pipelineInput[fileID] = thermalImagePath;

        // use the overloaded function for processing multiple images
        std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> multipleFileResults;
        if (RecognizeFumaroles(pipelineInput, multipleFileResults))
        {
            results = std::move(multipleFileResults[fileID]);
            return true;
        }

        return false;
    }

    bool FumaroleRecognizer::RecognizeFumaroles(const std::map<std::string, std::string> &files, std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> &results) const
    {
        // create new pipelines on image and run the result
        Pipeline::Pipeline p1(files, Model::FumaroleType::FUMAROLE_VERY_HOT);
        Pipeline::Pipeline p2(files, Model::FumaroleType::FUMAROLE_HOT);

        // run pipelines
        bool p1Success = p1.Run();
        bool p2Success = p2.Run();

        if (p1Success && p2Success)
        {
            // convert localizations of pipelines into results
            std::map<std::string, std::vector<FumaroleDetectionResult>> results1 = std::move(
                    ConvertLocalizations(p1.GetLocalizations()));
            std::map<std::string, std::vector<FumaroleDetectionResult>> results2 = std::move(
                    ConvertLocalizations(p2.GetLocalizations()));

            // merge results of both heat pipelines
            DetectionMap mergedMap = MergeResults(results1, results2);

            return true;
        }

        return false;
    }

    // Convert localizations from pipeline into detection results
    std::map<std::string, std::vector<FumaroleDetectionResult>> FumaroleRecognizer::ConvertLocalizations(const Pipeline::PipelineLocalizations &localizations) const
    {
        std::map<std::string, std::vector<FumaroleDetectionResult>> results;
        cv::Rect rect;
        FumaroleDetectionResult detectionResult;

        // process each image result
        for (auto localization : localizations)
        {
            // process each set of contour points for this image result
            for (auto contours : localization.second)
            {
                // convert to rect and add to this file's list
                rect = std::move(cv::boundingRect(contours));
                detectionResult.ImageID = localization.first;
                detectionResult.BoundingBox = std::move(rect);
                results[localization.first].emplace_back(std::move(detectionResult));
            }
        }

        return std::move(results);
    }

    // Merge detections maps
    // m1: the high heat map
    // m2: the lower heat map
    DetectionMap FumaroleRecognizer::MergeResults(const Recognition::DetectionMap &m1, const Recognition::DetectionMap &m2) const
    {
        DetectionMap mergedMap;
        std::vector<FumaroleDetectionResult> m2Detections;

        for (std::map<std::string, std::vector<FumaroleDetectionResult>>::const_iterator iter = m1.begin(); iter != m1.end(); iter++)
        {
            // check if the lower heat map also has a detection for this file
            auto iter2 = m2.find(iter->first);
            if (iter2 != m2.end())
            {
                // there is - if there is a bounding box that encloses one in the hot, remove it
                m2Detections = iter2->second;
                for (const FumaroleDetectionResult& r1 : iter->second)
                {
                    for (auto m2DetectionsIter = m2Detections.begin(); m2DetectionsIter != m2Detections.end(); m2DetectionsIter++)
                    {
                        if ((r1.BoundingBox & m2DetectionsIter->BoundingBox) == r1.BoundingBox) {
                            m2DetectionsIter = m2Detections.erase(m2DetectionsIter);
                            continue;
                        }
                    }

                    mergedMap[iter->first].push_back(r1);
                }

                // add all m2 detections that were not removed (these are stand-alone and do not overlap the hotter fumaroles)
                mergedMap[iter->first].insert(mergedMap[iter->first].end(),  m2Detections.begin(), m2Detections.end());
            }
            else
            {
                // detections are separate, just add to merged map
                mergedMap[iter->first] = iter->second;
            }
        }

        // add remaining detections in m2 to the merged map
        for (std::map<std::string, std::vector<FumaroleDetectionResult>>::const_iterator iter = m2.begin(); iter != m2.end(); iter++)
        {
            if (mergedMap.find(iter->first) != mergedMap.end()) {
                mergedMap[iter->first] = iter->second;
            }
        }

        return std::move(mergedMap);
    }

    // Save results as images
    void FumaroleRecognizer::SaveResults(const Recognition::DetectionMap &resultMap) const
    {
        // create output dir if needed
        if (!std::filesystem::exists(Config::FINAL_RESULTS_OUTPUT_DIR)) {
            std::filesystem::create_directory(Config::FINAL_RESULTS_OUTPUT_DIR);
        }

        // save all images
        cv::Mat image;
        std::string path;

        for (const auto& result : resultMap)
        {
            // load the original grayscale image
            IO::GetThermalImage(result.first, image, true);

            // draw all detected bounding boxes on this image
            for (const FumaroleDetectionResult& r : result.second) {
                cv::rectangle(image, r.BoundingBox, r.Type == Model::FumaroleType::FUMAROLE_VERY_HOT ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 166, 255), 2);
            }

            path = Config::FINAL_RESULTS_OUTPUT_DIR;
            path += result.first;
            path += Config::IMAGE_OUTPUT_EXT;

            cv::imwrite(path, image);
        }
    }
}
