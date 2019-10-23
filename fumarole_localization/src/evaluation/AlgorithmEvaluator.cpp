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

                // accumulate total error
                eval.Error += singleImageEval.Error;
            }
        }

        return std::move(eval);
    }

    // Evaluate single image results
    FumaroleDetectionEvaluation AlgorithmEvaluator::EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult> &results, const std::vector<Recognition::FumaroleDetectionResult> &truth) const
    {
        FumaroleDetectionEvaluation eval;

        // convert all bounding boxes to eigen vectors
        std::vector<Eigen::Vector4f> vectors;
        ConvertResultsToEigenVectors(results, vectors);

        std::vector<Eigen::Vector4f> truthVectors;
        ConvertResultsToEigenVectors(truth, truthVectors);

        eval.NumberDetected = results.size();
        eval.NumberOfActualFumaroles = truth.size();

        std::vector<float> l1Min;        // min l1 distances
        std::vector<float> l1Temp;       // l1 distances
        cv::Mat image;

        for (const auto& x : vectors)
        {
            // try and find the corresponding rect in the ground truth list
            for (const auto& y : truthVectors) {
                float l1Distance = abs((x - y).lpNorm<1>());
                l1Temp.emplace_back(l1Distance);
            }

            // the min distance is the corresponding vector
            auto minIter = std::min_element(l1Temp.begin(), l1Temp.end());
            l1Min.emplace_back(*minIter);

            // clear for next round
            l1Temp.clear();
        }

        // sum min l1 error
        eval.Error = std::accumulate(l1Min.begin(), l1Min.end(), 0.0);

        return eval;
    }

    // Convert list of results to Eigen vectors
    void AlgorithmEvaluator::ConvertResultsToEigenVectors(const std::vector<Recognition::FumaroleDetectionResult>& results, std::vector<Eigen::Vector4f> &vectors) const
    {
        for (auto r : results) {
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
