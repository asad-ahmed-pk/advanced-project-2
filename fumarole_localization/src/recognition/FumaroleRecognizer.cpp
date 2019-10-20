//
// FumaroleRecognizer.cpp
// Takes input for fumarole thermal images and recognizes fumaroles
//

#include "recognition/FumaroleRecognizer.hpp"
#include "model/FumaroleType.hpp"

#include <map>
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
        p1.Run();
        p2.Run();

        // convert localizations of pipelines into results
        std::map<std::string, std::vector<FumaroleDetectionResult>> results1 = std::move(ConvertLocalizations(p1.GetLocalizations()));
        std::map<std::string, std::vector<FumaroleDetectionResult>> results2 = std::move(ConvertLocalizations(p2.GetLocalizations()));

        // TODO: merge results of both heat pipelines
        return true;

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

        // TODO: return merged map
        return std::move(mergedMap);
    }
}
