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
#include <map>

namespace Evaluation
{
    class AlgorithmEvaluator
    {
    public:
        AlgorithmEvaluator();

        ~AlgorithmEvaluator();

        /// Evaluate the results from running the detection algorithm on a set of images
        /// \param results The results of running the algorithm on images where <file id: list of detections>
        /// \param truth The ground thruth data for each file where <file id: ground truth detection>
        /// \return An evaluation of the algorithm
        AlgorithmEvaluation EvaluateDetectionPipeline(const std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>>& results, const std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>>& truth) const;

        /// Evaluate the results detected for a single image
        /// \param results The list of detected fumaroles
        /// \param truth The ground truth to evaluate against
        /// \return Returns the evaluation result
        FumaroleDetectionEvaluation EvaluateDetections(const std::vector<Recognition::FumaroleDetectionResult>& results, const std::vector<Recognition::FumaroleDetectionResult>& truth) const;

        void DrawDetectionsVsGroundtruth(const std::string& fileID, const std::vector<Recognition::FumaroleDetectionResult>& results, const std::vector<Recognition::FumaroleDetectionResult>& truth) const;

    private:
        void ConvertResultsToEigenVectors(const std::vector<Recognition::FumaroleDetectionResult>& results, std::vector<Eigen::Vector4f>& vectors) const;
        float ComputeIoU(const cv::Rect& r1, const cv::Rect& r2) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_H
