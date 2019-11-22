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

const std::string FOLDER { "test_set_3/" };

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
    Evaluation::AlgorithmEvaluator evaluator;
    Evaluation::AlgorithmEvaluation eval = evaluator.EvaluateDetectionPipeline(results, groundTruth);

    const float precision = static_cast<float>(eval.TotalNumberDetected) / static_cast<float>(eval.TotalNumberOfActualFumaroles) * 100;

    // print out evaluation results
    std::cout << "\n\n--------------- Algorithm Evaluation ---------------\n";
    std::cout << "\nTotal number of detections = " << eval.TotalNumberDetected;
    std::cout << "\nActual number of detections = " << eval.TotalNumberOfActualFumaroles;
    std::cout << "\nTotal Average IoU = " << eval.TotalAverageIoU;
    std::cout << "\nTotal Accuracy (%) = " << eval.TotalAverageIoU * 100;
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

    for (const Evaluation::FumaroleDetectionEvaluation& e : eval.Evaluations)
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

    // draw the results vs ground truth for comparison
    std::cout << "\n\nSaving ground truth vs results" << std::endl;
    for (const auto& y : groundTruth) {
        evaluator.DrawDetectionsVsGroundtruth(y.first, FOLDER, results[y.first], y.second);
    }

    std::cout << std::endl;

    return 0;
}