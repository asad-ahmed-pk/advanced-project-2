//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole recognition
//

#include "evaluation/AlgorithmEvaluator.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <eigen3/Eigen/Eigen>

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
}
