//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole detection
//

#ifndef FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_HPP
#define FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_HPP

#include "detection/FumaroleDetection.hpp"
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
        AlgorithmEvaluation EvaluateDetectionPipeline(const std::map<std::string, std::vector<Detection::FumaroleDetection>>& results, const std::map<std::string, std::vector<Detection::FumaroleDetection>>& truth) const;

        /// Evaluate the results detected for a single image
        /// \param results The list of detected fumaroles
        /// \param truth The ground truth to evaluate against
        /// \return Returns the evaluation result
        FumaroleDetectionEvaluation EvaluateDetections(const std::vector<Detection::FumaroleDetection>& results, const std::vector<Detection::FumaroleDetection>& truth) const;

        /// Draw the detections (red) vs the ground truth (blue) bounding boxes
        /// \param fileID The ID of the file
        /// \param folder The folder name to group this result in
        /// \param results A list of detection results
        /// \param truth A list of ground truth results
        void DrawDetectionsVsGroundtruth(const std::string& fileID, const std::string& folder, const std::vector<Detection::FumaroleDetection>& results, const std::vector<Detection::FumaroleDetection>& truth) const;

        /// Save the detection metrics data to a CSV file
        /// \param filePath The full path (including file extension) for the CSV file
        /// \param metrics The metrics data for the detection
        static void SaveDetectionEvaluationMetricsToCSV(const std::string filePath, const std::map<int, std::tuple<int, int>>& metrics);

        static void SaveIoUEvaluationMetricsToCSV(const std::string filePath, const std::map<float, float>& metrics);

    private:
        void ConvertResultsToEigenVectors(const std::vector<Detection::FumaroleDetection>& results, std::vector<Eigen::Vector4f>& vectors) const;
        void ConvertResultsToEigenVectors(const std::vector<Detection::FumaroleDetection>& detections, std::vector<Eigen::Vector2f>& vectors) const;
        Eigen::MatrixX2f ConvertToCentroidEigenMatrix(const std::vector<Detection::FumaroleDetection>& detections) const;
        void ComputeDetectionMetrics(const std::vector<Detection::FumaroleDetection>& results, const std::vector<Detection::FumaroleDetection>& groundTruth, std::map<int, std::tuple<int, int>>& metrics) const;
        float ComputeIoU(const cv::Rect& r1, const cv::Rect& r2) const;
        void ComputeIoUMetrics(const std::vector<float>& iouScores, std::map<float, float>& metrics) const;

    private:
        int m_DetectionThresholdMin;
        int m_DetectionThresholdMax;
        int m_DetectionThresholdStep;
        float m_IoUThresholdStep;
    };
}

#endif //FUMAROLE_LOCALIZATION_ALGORITHMEVALUATOR_HPP
