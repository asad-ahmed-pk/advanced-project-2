//
// AlgorithmEvaluator.h
// Evaluates the performance of the fumarole detection
//

#include "evaluation/AlgorithmEvaluator.hpp"
#include "io/fumarole_data_io.hpp"
#include "config/ConfigParser.hpp"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <eigen3/Eigen/Eigen>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

namespace Evaluation
{
    AlgorithmEvaluator::AlgorithmEvaluator()
    {
        // read in threshold params from XML file for evaluation
        m_DetectionThresholdMin = Config::ConfigParser::GetInstance().GetValue<int>("config.evaluation.detection.threshold_min");
        m_DetectionThresholdMax = Config::ConfigParser::GetInstance().GetValue<int>("config.evaluation.detection.threshold_max");
        m_DetectionThresholdStep = Config::ConfigParser::GetInstance().GetValue<int>("config.evaluation.detection.threshold_step");
        m_IoUThresholdStep = Config::ConfigParser::GetInstance().GetValue<float>("config.evaluation.detection.iou_threshold_step");
    }

    AlgorithmEvaluator::~AlgorithmEvaluator()
    {

    }

    // Evaluate whole pipeline
    AlgorithmEvaluation AlgorithmEvaluator::EvaluateDetectionPipeline(
            const std::map<std::string, std::vector<Detection::FumaroleDetection>> &results,
            const std::map<std::string, std::vector<Detection::FumaroleDetection>> &truth) const
    {
        AlgorithmEvaluation eval;
        FumaroleDetectionEvaluation singleImageEval;

        float totalIoU = 0.0;

        for (const auto& truthResult : truth)
        {
            eval.TotalNumberOfActualFumaroles += truthResult.second.size();

            // get corresponding detection from the pipeline
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

                // accumulate total detection metrics
                for (int t = m_DetectionThresholdMin; t <= m_DetectionThresholdMax; t += m_DetectionThresholdStep) {
                    std::get<0>(eval.DetectionMetrics[t]) += std::get<0>(singleImageEval.DetectionMetrics[t]);
                    std::get<1>(eval.DetectionMetrics[t]) += std::get<1>(singleImageEval.DetectionMetrics[t]);
                }

                // sum up confusion matrix for overall score
                eval.ConfusionMatrix += singleImageEval.ConfusionMatrix;
            }
        }

        // average the detection metrics for the IoUs
        if (!eval.Evaluations.empty())
        {
            for (float t = 0.0; t <= 1.0; t += m_IoUThresholdStep)
            {
                eval.IoUDetectionMetrics[t] = std::accumulate(eval.Evaluations.begin(), eval.Evaluations.end(), 0.0, [=](float total, Evaluation::FumaroleDetectionEvaluation &d) -> float {
                    total += d.IoUMetrics[t];
                    return total;
                }) / static_cast<float>(eval.Evaluations.size());
            }
        }

        // total average IoU for whole algorithm considering all detections in all images
        eval.TotalAverageIoU = totalIoU / static_cast<float>(eval.Evaluations.size());

        return std::move(eval);
    }

    // Evaluate single image results
    FumaroleDetectionEvaluation AlgorithmEvaluator::EvaluateDetections(const std::vector<Detection::FumaroleDetection> &results, const std::vector<Detection::FumaroleDetection> &truth) const
    {
        FumaroleDetectionEvaluation eval;

        eval.NumberDetected = results.size();
        eval.NumberOfActualFumaroles = truth.size();

        // compute ious with each one in the ground truth dataset
        std::vector<float> ious;
        std::vector<float> correspondingIOUs;

        for (const auto& x : results)
        {
            for (const auto& y : truth) {
                float iou = ComputeIoU(x.BoundingBox, y.BoundingBox);
                ious.push_back(iou);
            }

            // set the max iou from the list as the valid IOU
            // ie; this was the corresponding rect to compare to in the ground truth set
            auto maxIter = std::max_element(ious.begin(), ious.end());
            correspondingIOUs.push_back(*maxIter);

            // detection evaluation
            ComputeDetectionMetrics(results, truth, eval.DetectionMetrics);

            // classification evaluation
            size_t truthIndex = std::distance(ious.begin(), maxIter);
            eval.ConfusionMatrix.AddClassifications(Model::TypeNameString(x.Type), Model::TypeNameString(truth[truthIndex].Type));

            // clear for matching next result
            ious.clear();
        }

        // Compute IoU metrics
        ComputeIoUMetrics(correspondingIOUs, eval.IoUMetrics);

        float n = correspondingIOUs.size() > 0 ? static_cast<float>(correspondingIOUs.size()) : 1.0;
        eval.AverageIoU = std::accumulate(correspondingIOUs.begin(), correspondingIOUs.end(), 0.0) / n;

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
    void AlgorithmEvaluator::ConvertResultsToEigenVectors(const std::vector<Detection::FumaroleDetection>& results, std::vector<Eigen::Vector4f> &vectors) const
    {
        for (const auto& r : results) {
            vectors.emplace_back(Eigen::Vector4f(r.BoundingBox.x, r.BoundingBox.y, r.BoundingBox.x + r.BoundingBox.width, r.BoundingBox.y + r.BoundingBox.height));
        }
    }

    // Compute detection metrics for true positives and false positives
    void AlgorithmEvaluator::ComputeDetectionMetrics(const std::vector<Detection::FumaroleDetection> &results,
                                                     const std::vector<Detection::FumaroleDetection>& groundTruth,
                                                     std::map<int, std::tuple<int, int>> &metrics) const
    {
        // convert to eigen matrix (library eigen - and not the mathematical concept of eigen vectors)
        Eigen::MatrixX2f X = ConvertToCentroidEigenMatrix(results);
        Eigen::MatrixX2f Y = ConvertToCentroidEigenMatrix(groundTruth);

        // counts
        int truePositives = 0;
        int falsePositives = 0;
        int count = 0;

        // run iterations based on threshold params and build up map of FP and TP
        for (int t = m_DetectionThresholdMin; t <= m_DetectionThresholdMax; t += m_DetectionThresholdStep)
        {
            // reset counts for this threshold
            truePositives = 0;
            falsePositives = 0;

            for (int i = 0; i < Y.rows(); i++)
            {
                // get l2 distances of Yi with each Xi
                Eigen::VectorXf distances = (X.rowwise() - Y.row(i)).rowwise().squaredNorm();

                // count distances to obtain the points that fall in the threshold
                count = (distances.array() <= t).count();
                if (count > 0) {
                    truePositives++;
                    falsePositives += (count - 1);       // since 1 count is for true positive (the min element)
                }
            }

            std::get<0>(metrics[t]) = truePositives;
            std::get<1>(metrics[t]) = falsePositives;
        }
    }

    // Compute IoU metrics against varying thresholds
    void AlgorithmEvaluator::ComputeIoUMetrics(const std::vector<float>& iouScores, std::map<float, float> &metrics) const
    {
        // run threshold range
        std::vector<float> validIoUs;
        float successRate = 0.0;

        for (float t = 0.0; t <= 1.0; t += m_IoUThresholdStep)
        {
            std::copy_if(iouScores.begin(), iouScores.end(), std::back_inserter(validIoUs), [=](const float iou) { return iou >= t; });
            if (!iouScores.empty()) {
                successRate = static_cast<float>(validIoUs.size()) / static_cast<float>(iouScores.size());
            }

            metrics[t] = successRate;

            validIoUs.clear();
            successRate = 0.0;
        }
    }

    // Convert std::vector of detections to eigen matrix of centroids of detections
    Eigen::MatrixX2f AlgorithmEvaluator::ConvertToCentroidEigenMatrix(const std::vector<Detection::FumaroleDetection> &detections) const
    {
        Eigen::MatrixX2f matrix(detections.size(), 2);
        int row = 0;

        for (const Detection::FumaroleDetection& d : detections)
        {
            matrix(row, 0) = d.Center().x;
            matrix(row, 1) = d.Center().y;

            row++;
        }

        return std::move(matrix);
    }

    // Convert bounding boxes to centroid eigen vectors
    void AlgorithmEvaluator::ConvertResultsToEigenVectors(const std::vector<Detection::FumaroleDetection> &results,
                                                          std::vector<Eigen::Vector2f> &vectors) const
    {
        for (const Detection::FumaroleDetection& d : results) {
            vectors.emplace_back(Eigen::Vector2f(d.Center().x, d.Center().y));
        }
    }

    // Save metrics to CSV file
    void AlgorithmEvaluator::SaveDetectionEvaluationMetricsToCSV(const std::string filePath,
                                                                 const std::map<int, std::tuple<int, int>> &metrics)
    {
        std::ofstream fs;
        fs.open(filePath, std::ios::out);

        // write header
        fs << "Threshold," << "TP," << "FP," << "Precision";
        float denom = 0.0;

        // write all thresholds and the TP and FPs for each threshold on a row
        for (const auto& pair : metrics)
        {
            denom = static_cast<float>(std::get<0>(pair.second)) + static_cast<float>(std::get<1>(pair.second));

            fs << "\n" << pair.first << ",";
            fs << std::get<0>(pair.second) << ",";
            fs << std::get<1>(pair.second) << ",";
            fs << static_cast<float>(std::get<0>(pair.second)) / (denom > 0.0 ? denom : 1.0);
        }

        fs.close();
    }

    // Save IoU metrics to CSV
    void AlgorithmEvaluator::SaveIoUEvaluationMetricsToCSV(const std::string filePath, const std::map<float, float> &metrics)
    {
        std::ofstream fs;
        fs.open(filePath, std::ios::out);

        // write header
        fs << "Threshold," << "Success Rate";

        // write the threshold and success rate to CSV file
        for (const auto& pair : metrics) {
            fs << "\n" << pair.first << "," << pair.second;
        }

        fs.close();
    }

    // Draw bounding boxes of both detection and ground truths for comparison
    void AlgorithmEvaluator::DrawDetectionsVsGroundtruth(const std::string& fileID,
                                                         const std::string& folder,
                                                         const std::vector<Detection::FumaroleDetection> &results,
                                                         const std::vector<Detection::FumaroleDetection> &truth) const
    {
        // draw the bounding boxes of detections vs the ground truth bounding boxes
        cv::Mat image;
        if (!IO::GetThermalImage(fileID, image, true)) {
            std::cerr << "\nFailed to read image: " << fileID << " for evaluation" << std::endl;
        }

        // draw ground truth bounding boxes
        for (const auto& y : truth) {
            cv::rectangle(image, y.BoundingBox, cv::Scalar(255, 0, 0));
        }

        // draw results' bounding box
        for (const auto& x : results) {
            cv::rectangle(image, x.BoundingBox, cv::Scalar(0, 0, 255));
        }

        std::string savePath = Config::EVALUATION_IMAGES_OUTPUT_DIR + folder;
        boost::filesystem::path path(savePath);
        if (!boost::filesystem::exists(path)) {
            boost::filesystem::create_directories(path);
        }
        savePath += fileID;
        savePath += Config::IMAGE_OUTPUT_EXT;

        cv::imwrite(savePath, image);
    }
}
