// main.cpp
// Main executable

#include <string>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "model/FumaroleType.hpp"
#include "pipeline/Pipeline.hpp"
#include "io/fumarole_data_io.hpp"

int main(int argc, char** argv)
{
    // get files
    std::map<std::string, std::string> files = {
            { "000503014000", "../../../Data/0008/CamThermal/CamThermal_000503014000.exr" },
            { "001837380000", "../../../Data/0009/CamThermal/CamThermal_001837380000.exr" },
            { "001839080000", "../../../Data/0009/CamThermal/CamThermal_001839080000.exr" },
            { "001862580000", "../../../Data/0009/CamThermal/CamThermal_001862580000.exr" },
            { "001944180000", "../../../Data/0009/CamThermal/CamThermal_001944180000.exr" }
    };
    //IO::GetGreyscaleHeatMaps(files);

    // construct pipeline
    std::cout << "\n\nRunning pipeline" << std::endl;

    Pipeline::Pipeline pipeline(files);
    pipeline.Run();

    std::cout << "\nDone" << std::endl;

    return 0;
}

