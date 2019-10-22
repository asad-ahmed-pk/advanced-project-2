//
// Evaluation.h
// Represents the results of evaluating detection of fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_EVALUATION_HPP
#define FUMAROLE_LOCALIZATION_EVALUATION_HPP

#include <string>

// Note: Localization accuracy models how accurate the algorithm matches a fumarole to its ground truth
// Detection accuracy models how accurate it is with regards to the number of detections

/// Evaluation on a single image
struct FumaroleDetectionEvaluation
{
    std::string ImageID;
    int NumberOfActualFumaroles;
    int NumberDetected;
    double DetectionAccuracy;
    double LocalizationAccuracy;
};

/// Evaluation on multiple images (the entire detection algorithm)
struct AlgorithmEvaluation
{
    int TotalNumberOfActualFumaroles;
    int TotalNumberDetected;
    double DetectionAccuracy;
    double LocalizationAccuracy;
};

#endif //FUMAROLE_LOCALIZATION_EVALUATION_HPP
