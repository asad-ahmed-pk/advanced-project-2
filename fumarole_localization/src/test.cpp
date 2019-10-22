// test.cpp
// Test program for testing the pipeline / detector against ground truth dataset

#include "io/DatasetLoader.hpp"
#include "io/fumarole_data_io.hpp"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

int main(int argc, char** argv)
{
    // Load test files and ground truth
    std::map<std::string, std::string> testFiles;
    std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>> groundTruth;
    IO::DatasetLoader::LoadTestData(testFiles, groundTruth);

    for (auto& iter : groundTruth)
    {
        std::cout << "\n\n" << iter.first << std::endl;
        for (const auto& result : iter.second) {
            std::cout << "\nx: " << result.BoundingBox.x << " y: " << result.BoundingBox.y << " width: " << result.BoundingBox.width << " height: " << result.BoundingBox.height << std::endl;
        }
    }

    return 0;
}