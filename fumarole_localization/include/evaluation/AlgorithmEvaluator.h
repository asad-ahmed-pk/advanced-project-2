//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole recognition
//

#ifndef FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H
#define FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H

#include "recognition/FumaroleDetectionResult.hpp"
#include "evaluation/Evaluation.hpp"

#include <vector>

namespace Evaluation
{
    class AlgorithmEvaluator
    {
        AlgorithmEvaluator();

        ~AlgorithmEvaluator();

        /// Evaluate the results detected for a single image
        /// \param results The list of detected fumaroles
        /// \param truth The ground truth to evaluate against
        /// \return Returns the evaluation result
        FumaroleDetectionEvaluation EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult>& results, const std::vector<Recognition::FumaroleDetectionResult>& truth) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H
