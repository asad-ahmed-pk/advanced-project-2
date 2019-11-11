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
    // view one file
    /*
    cv::Mat image = cv::imread("../data/CamThermal/CamThermal_000489814000.exr", cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cerr << "\nFailed to open EXR image :(" << std::endl;
        return -1;
    }

    cv::namedWindow("EXR Image");
    cv::imshow("EXR Image", image);
    cv::waitKey(0);
    */

    // get files
    std::map<std::string, std::string> files = {
            { "503014", "../../../Data/0008/CamThermal/CamThermal_000503014000.exr" },
            { "183738", "../../../Data/0009/CamThermal/CamThermal_001837380000.exr" },
            { "183908", "../../../Data/0009/CamThermal/CamThermal_001839080000.exr" },
            { "186258", "../../../Data/0009/CamThermal/CamThermal_001862580000.exr" },
            { "194418", "../../../Data/0009/CamThermal/CamThermal_001944180000.exr" }
    };
    //IO::GetGreyscaleHeatMaps(files);

    // construct pipeline
    std::cout << "\n\nRunning pipeline" << std::endl;

    Pipeline::Pipeline pipeline(files);
    pipeline.Run();

    std::cout << "\nDone" << std::endl;

    return 0;
}

