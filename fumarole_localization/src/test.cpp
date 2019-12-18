// test.cpp
// Test program for testing the pipeline / detector against ground truth dataset

#include "io/DatasetLoader.hpp"
#include "detection/FumaroleDetector.hpp"
#include "evaluation/Evaluation.hpp"
#include "evaluation/AlgorithmEvaluator.hpp"

#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <boost/filesystem.hpp>

const std::string FOLDER { "test_set_4/" };
const std::string CSV_SAVE_FOLDER { "confusion_matrix" };
const std::string DETECTION_METRICS_SAVE_FOLDER { "detection_metrics" };
const std::string CONFUSION_MATRIX_FILE_NAME { "classification_confusion_matrix" };

// Eval functions for detector and classifier
void EvaluateDetector(const Evaluation::AlgorithmEvaluation& evaluation);
void EvaluateClassifier(const Evaluation::AlgorithmEvaluation& evaluation);
void SaveConfusionMatrices(const Evaluation::AlgorithmEvaluation& evaluation);
void SaveDetectionsMetrics(const Evaluation::AlgorithmEvaluation& evaluation);

int main(int argc, char** argv)
{
    // Load test files and ground truth
    std::map<std::string, std::string> testFiles;
    std::map<std::string, std::vector<Detection::FumaroleDetection>> groundTruth;
    IO::DatasetLoader::LoadTestData(FOLDER, testFiles, groundTruth);

    // run detection
    std::map<std::string, std::vector<Detection::FumaroleDetection>> results;

    // save the results
    Detection::FumaroleDetector detector;
    detector.DetectFumaroles(testFiles, results);
    detector.SaveResults(results);

    // evaluate the results
    std::cout << "\n\nRunning evaluation..." << std::endl;
    Evaluation::AlgorithmEvaluator evaluator;
    Evaluation::AlgorithmEvaluation eval = evaluator.EvaluateDetectionPipeline(results, groundTruth);

    // draw the results vs ground truth for comparison
    std::cout << "\n\nSaving ground truth vs results" << std::endl;
    for (const auto& y : groundTruth) {
        evaluator.DrawDetectionsVsGroundtruth(y.first, FOLDER, results[y.first], y.second);
    }

    // print stats for detector and classifier performance
    EvaluateDetector(eval);
    EvaluateClassifier(eval);

    // save detection metrics to csv files
    SaveDetectionsMetrics(eval);

    // save confusion matrices to csv
    SaveConfusionMatrices(eval);

    std::cout << std::endl;

    return 0;
}

// Evaluate detection
void EvaluateDetector(const Evaluation::AlgorithmEvaluation& evaluation)
{
    const float precision = static_cast<float>(evaluation.TotalNumberDetected) / static_cast<float>(evaluation.TotalNumberOfActualFumaroles) * 100;

    // print out evaluation results
    std::cout << "\n\n--------------- Detector Evaluation ---------------\n";
    std::cout << "\nTotal number of detections = " << evaluation.TotalNumberDetected;
    std::cout << "\nActual number of detections = " << evaluation.TotalNumberOfActualFumaroles;
    std::cout << "\nTotal Average IoU = " << evaluation.TotalAverageIoU;
    std::cout << "\nTotal Accuracy (%) = " << evaluation.TotalAverageIoU * 100;
    std::cout << "\nTotal Precision (%) = " << precision;
    if (precision > 100) {
        std::cout << " (Over detected)";
    }
    std::cout << std::endl;

    // print out individual results
    std::cout << "\n--------------- Image Evaluation ---------------\n";
    std::cout << std::setw(12) << std::setfill(' ') << "\nImage ID";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Avg IoU";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Accuracy %";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Detected";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Actual";
    std::cout << std::endl;

    for (const Evaluation::FumaroleDetectionEvaluation& e : evaluation.Evaluations)
    {
        std::cout << std::setw(12) << std::setfill(' ') << e.ImageID;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << e.AverageIoU;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << e.AverageIoU * 100;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << e.NumberDetected;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << e.NumberOfActualFumaroles;

        std::cout << std::endl;
    }
}

// Evaluate classifier
void EvaluateClassifier(const Evaluation::AlgorithmEvaluation& evaluation)
{
    // print out overall classification results
    std::cout << "\n\n--------------- Classifier Evaluation ---------------\n";
    std::cout << "\nOverall Accuracy Rate (%) = " << evaluation.ConfusionMatrix.GetAccuracy() * 100;
    std::cout << "\nOverall Misclassification Rate (%) = " << (1.0 - evaluation.ConfusionMatrix.GetAccuracy()) * 100;
    std::cout << std::endl;

    // print out individual results per image
    std::cout << "\n--------------- Image Evaluation ---------------\n";
    std::cout << std::setw(12) << std::setfill(' ') << "\nImage ID";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Accuracy";
    std::cout << std::setw(10) << std::setfill(' ') << " ";
    std::cout << std::setw(10) << std::setfill(' ') << "Misclassification Rate";
    std::cout << std::endl;

    for (const Evaluation::FumaroleDetectionEvaluation& e : evaluation.Evaluations)
    {
        std::cout << std::setw(12) << std::setfill(' ') << e.ImageID;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << e.ConfusionMatrix.GetAccuracy() * 100;
        std::cout << std::setw(10) << std::setfill(' ') << " ";
        std::cout << std::setw(10) << std::setfill(' ') << (1.0 - e.ConfusionMatrix.GetAccuracy()) * 100;

        std::cout << std::endl;
    }
}

// Save confusion matrices as CSVs
void SaveConfusionMatrices(const Evaluation::AlgorithmEvaluation& evaluation)
{
    // create folder if required
    const std::string folderPath = CSV_SAVE_FOLDER + "/" + FOLDER;
    if (!boost::filesystem::exists(folderPath)) {
        boost::filesystem::create_directories(folderPath);
    }

    // save overall algo classification confusion matrix
    std::ofstream fs;
    fs.open(folderPath + CONFUSION_MATRIX_FILE_NAME + ".csv", std::ios::out);
    fs << evaluation.ConfusionMatrix;
    fs.close();

    // save individual confusion matrices
    for (const Evaluation::FumaroleDetectionEvaluation& e : evaluation.Evaluations)
    {
        std::ofstream fs;
        fs.open(folderPath + e.ImageID + ".csv", std::ios::out);
        fs << e.ConfusionMatrix;
        fs.close();
    }
}

// Write detection metrics to CSV files
void SaveDetectionsMetrics(const Evaluation::AlgorithmEvaluation& evaluation)
{
    std::cout << "\nSaving detection metrics for single images" << std::endl;

    // create folder if needed
    if (!boost::filesystem::exists(DETECTION_METRICS_SAVE_FOLDER)) {
        boost::filesystem::create_directories(DETECTION_METRICS_SAVE_FOLDER);
    }

    std::string filePath;
    for (const Evaluation::FumaroleDetectionEvaluation& e : evaluation.Evaluations)
    {
        filePath = DETECTION_METRICS_SAVE_FOLDER + "/" + e.ImageID + ".csv";
        Evaluation::AlgorithmEvaluator::SaveDetectionEvaluationMetricsToCSV(filePath, e.DetectionMetrics);
    }

    std::cout << "\nSaving total detection metrics to CSV file" << std::endl;
    filePath = DETECTION_METRICS_SAVE_FOLDER + "/total_metrics.csv";
    Evaluation::AlgorithmEvaluator::SaveDetectionEvaluationMetricsToCSV(filePath, evaluation.DetectionMetrics);
}
