//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole recognition
//

#ifndef FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H
#define FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H

#include "recognition/FumaroleDetectionResult.hpp"
#include "evaluation/Evaluation.hpp"

#include <eigen3/Eigen/Eigen>
#include <vector>

namespace Evaluation
{
    class AlgorithmEvaluator
    {
    public:
        AlgorithmEvaluator();

        ~AlgorithmEvaluator();

        /// Evaluate the results detected for a single image
        /// \param results The list of detected fumaroles
        /// \param truth The ground truth to evaluate against
        /// \return Returns the evaluation result
        FumaroleDetectionEvaluation EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult>& results, const std::vector<Recognition::FumaroleDetectionResult>& truth) const;

    private:
        void ConvertResultsToEigenVectors(const std::vector<Recognition::FumaroleDetectionResult>& results, std::vector<Eigen::Vector4f>& vectors) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H
