//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole recognition
//

#include "evaluation/AlgorithmEvaluator.h"

#include <eigen3/Eigen/Eigen>

namespace Evaluation
{
    AlgorithmEvaluator::AlgorithmEvaluator()
    {

    }

    AlgorithmEvaluator::~AlgorithmEvaluator()
    {

    }

    FumaroleDetectionEvaluation AlgorithmEvaluator::EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult> &results, const std::vector<Recognition::FumaroleDetectionResult> &truth) const
    {
        FumaroleDetectionEvaluation eval;

        Eigen::Vector4f a;
        Eigen::Vector4f b;

        for (const auto& result : results)
        {
            // TODO: try and find the corresponding rect in the ground truth list

        }
    }
}
