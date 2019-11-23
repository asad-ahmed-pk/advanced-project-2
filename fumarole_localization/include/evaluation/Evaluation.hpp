//
// Evaluation.h
// Represents the results of evaluating detection of fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_EVALUATION_HPP
#define FUMAROLE_LOCALIZATION_EVALUATION_HPP

#include <string>
#include <vector>

#include "model/FumaroleType.hpp"
#include "ConfusionMatrix.hpp"

namespace Evaluation
{
    // Note: Localization accuracy models how accurate the algorithm matches a fumarole to its ground truth
    // Detection accuracy models how accurate it is with regards to the number of detections

    /// Evaluation on a single image
    struct FumaroleDetectionEvaluation
    {
        std::string ImageID = "n/a";
        int NumberOfActualFumaroles = 0;
        int NumberDetected = 0;
        float AverageIoU = 0.0;
        ConfusionMatrix ConfusionMatrix;

        FumaroleDetectionEvaluation();
    };

    /// Evaluation on multiple images (the entire detection algorithm)
    struct AlgorithmEvaluation
    {
        int TotalNumberOfActualFumaroles = 0;
        int TotalNumberDetected = 0;
        float TotalAverageIoU = 0.0;
        std::vector<FumaroleDetectionEvaluation> Evaluations;
    };
}

#endif //FUMAROLE_LOCALIZATION_EVALUATION_HPP
