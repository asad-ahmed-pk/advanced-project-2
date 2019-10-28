// test.cpp
// Test program for testing the pipeline / detector against ground truth dataset

#include "io/DatasetLoader.hpp"
#include "recognition/FumaroleRecognizer.hpp"
#include "evaluation/Evaluation.hpp"
#include "evaluation/AlgorithmEvaluator.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

const std::string FOLDER { "test_set_1/" };

int main(int argc, char** argv)
{
    // Load test files and ground truth
    std::map<std::string, std::string> testFiles;
    std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> groundTruth;
    IO::DatasetLoader::LoadTestData(FOLDER, testFiles, groundTruth);

    // run detection
    std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> results;

    // save the results
    Recognition::FumaroleRecognizer recognizer;
    recognizer.RecognizeFumaroles(testFiles, results);
    recognizer.SaveResults(results);

    // evaluate the results
    Evaluation::AlgorithmEvaluator evaluator;
    Evaluation::AlgorithmEvaluation eval = evaluator.EvaluateDetectionPipeline(results, groundTruth);

    // print out evaluation results
    std::cout << "\n\n--------------- Algorithm Evaluation ---------------\n";
    std::cout << "\nTotal number of detections = " << eval.TotalNumberDetected;
    std::cout << "\nActual number of detections = " << eval.TotalNumberOfActualFumaroles;
    std::cout << "\nTotal Average IoU = " << eval.TotalAverageIoU;
    std::cout << "\nTotal Accuracy (%) = " << eval.TotalAverageIoU * 100;

    // print out individual results
    std::cout << "\n\n--------------- Image Evaluation ---------------\n";
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