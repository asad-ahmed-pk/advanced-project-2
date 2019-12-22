// train.cpp
// training with 5 images used for training

#include "detection/FumaroleDetector.hpp"
#include "detection/FumaroleDetection.hpp"

#include <iostream>
#include <map>

int main(int argc, char** argv)
{
    // get files

    // standard thermal image
    /*
    std::map<std::string, std::string> files = {
            { "000503014000", "../../../Data/0008/CamThermal/CamThermal_000503014000.exr" },
            { "001837380000", "../../../Data/0009/CamThermal/CamThermal_001837380000.exr" },
            { "001839080000", "../../../Data/0009/CamThermal/CamThermal_001839080000.exr" },
            { "001862580000", "../../../Data/0009/CamThermal/CamThermal_001862580000.exr" },
            { "001944180000", "../../../Data/0009/CamThermal/CamThermal_001944180000.exr" }
    };
     */

    // rectified, PCL mapped thermals
    std::map<std::string, std::string> files = {
            { "000503014000", "../../../Data/0008/PCL_mappedImage_vis_grey/PCL_mappedImage_000503014000.bmp" },
            { "001837380000", "../../../Data/0009/PCL_mappedImage_vis_grey/PCL_mappedImage_001837380000.bmp" },
            { "001839080000", "../../../Data/0009/PCL_mappedImage_vis_grey/PCL_mappedImage_001839080000.bmp" },
            { "001862580000", "../../../Data/0009/PCL_mappedImage_vis_grey/PCL_mappedImage_001862580000.bmp" },
            { "001944180000", "../../../Data/0009/PCL_mappedImage_vis_grey/PCL_mappedImage_001944180000.bmp" }
    };

    // detector results
    std::map<std::string, std::vector<Detection::FumaroleDetection>> results;

    // create and run detector
    Detection::FumaroleDetector detector(true);
    detector.DetectFumaroles(files, results);

    // save results to disk
    detector.SaveResults(results);

    std::cout << "\nDone" << std::endl;

    return 0;
}

