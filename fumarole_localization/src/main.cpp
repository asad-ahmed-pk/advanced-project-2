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
    std::map<std::string, std::string> files;
    IO::GetGreyscaleHeatMaps(files);

    // construct pipelines
    std::cout << "\n\nRunning pipeline for very hot fumaroles" << std::endl;
    Pipeline::Pipeline pVeryHot(files, Model::FumaroleType::FUMAROLE_VERY_HOT);
    pVeryHot.Run();

    std::cout << "\n\nRunning pipeline for hot fumaroles" << std::endl;
    Pipeline::Pipeline pHot(files, Model::FumaroleType::FUMAROLE_HOT);
    pHot.Run();

    std::cout << "\nDone" << std::endl;

    return 0;
}

