//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole recognition
//

#include "evaluation/AlgorithmEvaluator.h"
#include "io/fumarole_data_io.hpp"

#include <algorithm>
#include <numeric>
#include <eigen3/Eigen/Eigen>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

namespace Evaluation
{
    AlgorithmEvaluator::AlgorithmEvaluator()
    {

    }

    AlgorithmEvaluator::~AlgorithmEvaluator()
    {

    }

    // Evaluate whole pipeline
    AlgorithmEvaluation AlgorithmEvaluator::EvaluateDetectionPipeline(
            const std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> &results,
            const std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> &truth) const
    {
        AlgorithmEvaluation eval;
        FumaroleDetectionEvaluation singleImageEval;

        float totalIoU = 0.0;

        for (const auto& truthResult : truth)
        {
            eval.TotalNumberOfActualFumaroles += truthResult.second.size();

            // get corresponding recognition from the pipeline
            auto iter = results.find(truthResult.first);
            if (iter != results.end())
            {
                eval.TotalNumberDetected += iter->second.size();

                // save the evaluation on this image
                singleImageEval = EvaluateDetections(iter->second, truthResult.second);
                singleImageEval.ImageID = truthResult.first;
                eval.Evaluations.emplace_back(singleImageEval);

                // accumulate total average IoU
                totalIoU += singleImageEval.AverageIoU;
            }
        }

        eval.TotalAverageIoU = totalIoU / static_cast<float>(eval.Evaluations.size());

        return std::move(eval);
    }

    // Evaluate single image results
    FumaroleDetectionEvaluation AlgorithmEvaluator::EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult> &results, const std::vector<Recognition::FumaroleDetectionResult> &truth) const
    {
        FumaroleDetectionEvaluation eval;

        eval.NumberDetected = results.size();
        eval.NumberOfActualFumaroles = truth.size();

        // compute ious with each one in the ground truth dataset
        std::vector<float> ious;
        std::vector<float> correspondingIOUs;

        for (const auto& result : results)
        {
            for (const auto& y : truth) {
                float iou = ComputeIoU(result.BoundingBox, y.BoundingBox);
                ious.push_back(iou);
            }

            // set the max iou from the list as the valid IOU
            // ie; this was the corresponding rect to compare to in the ground truth set
            auto maxIter = std::max_element(ious.begin(), ious.end());
            correspondingIOUs.push_back(*maxIter);

            // clear for matching next result
            ious.clear();
        }

        eval.AverageIoU = std::accumulate(correspondingIOUs.begin(), correspondingIOUs.end(), 0.0) / static_cast<float>(correspondingIOUs.size());

        return eval;
    }

    // Compute IoU of 2 rects
    float AlgorithmEvaluator::ComputeIoU(const cv::Rect &r1, const cv::Rect &r2) const
    {
        cv::Rect intersectionRect = r1 & r2;
        float intersectionArea = static_cast<float>(intersectionRect.area());

        if (intersectionRect.area() > 0)
        {
            cv::Rect unionRect = r1 | r2;

            float unionArea = static_cast<float>(unionRect.area());
            float iou = intersectionArea / unionArea;

            return iou;
        }

        return 0.0;
    }

    // Convert list of results to Eigen vectors
    void AlgorithmEvaluator::ConvertResultsToEigenVectors(const std::vector<Recognition::FumaroleDetectionResult>& results, std::vector<Eigen::Vector4f> &vectors) const
    {
        for (const auto& r : results) {
            vectors.emplace_back(Eigen::Vector4f(r.BoundingBox.x, r.BoundingBox.y, r.BoundingBox.x + r.BoundingBox.width, r.BoundingBox.y + r.BoundingBox.height));
        }
    }

    // Draw bounding boxes of both detection and ground truths for comparison
    void AlgorithmEvaluator::DrawDetectionsVsGroundtruth(const std::string &fileID,
                                                         const std::vector<Recognition::FumaroleDetectionResult> &results,
                                                         const std::vector<Recognition::FumaroleDetectionResult> &truth) const
    {
        // draw the bounding boxes of detections vs the ground truth bounding boxes
        cv::Mat image;
        IO::GetThermalImage(fileID, image, true);

        // draw ground truth bounding boxes
        for (const auto& y : truth) {
            cv::rectangle(image, y.BoundingBox, cv::Scalar(255, 0, 0));
        }

        // draw results' bounding box
        for (const auto& x : results) {
            cv::rectangle(image, x.BoundingBox, cv::Scalar(0, 0, 255));
        }

        std::string savePath = Config::EVALUATION_IMAGES_OUTOUT_DIR;
        if (!boost::filesystem::exists(savePath)) {
            boost::filesystem::create_directory(savePath);
        }
        savePath += fileID;
        savePath += Config::IMAGE_OUTPUT_EXT;

        cv::imwrite(savePath, image);
    }
}
