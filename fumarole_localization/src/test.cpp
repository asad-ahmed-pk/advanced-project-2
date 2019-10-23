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

int main(int argc, char** argv)
{
    // Load test files and ground truth
    std::map<std::string, std::string> testFiles;
    std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> groundTruth;
    IO::DatasetLoader::LoadTestData(testFiles, groundTruth);

    /*
    for (auto& iter : groundTruth)
    {
        std::cout << "\n\n" << iter.first << std::endl;
        for (const auto& result : iter.second) {
            std::cout << "\nx: " << result.BoundingBox.x << " y: " << result.BoundingBox.y << " width: " << result.BoundingBox.width << " height: " << result.BoundingBox.height << std::endl;
        }
    }
    */

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
    std::cout << "\n\n--------------- Evaluation ---------------\n";
    std::cout << "\nTotal number of detections = " << eval.TotalNumberDetected;
    std::cout << "\nActual number of detections = " << eval.TotalNumberOfActualFumaroles;
    std::cout << "\nTotal Error: " << eval.Error;

    std::cout << std::endl;

    return 0;
}