// main.cpp
// Main executable

#include <string>
#include <iostream>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "model/FumaroleType.hpp"
#include "pipeline/Pipeline.hpp"
#include "io/fumarole_data_io.hpp"

int main(int argc, char** argv)
{
    // test one image
    /*
    cv::Mat image = cv::imread("../data/CamThermal_vis_grey/CamThermal_000758414000.bmp", cv::IMREAD_GRAYSCALE);

    // view image before processing
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);
    */

    // get files
    std::map<std::string, std::string> files;
    IO::GetGreyscaleHeatMaps(files);

    // construct pipelines
    Pipeline::Pipeline pVeryHot(files, Model::FumaroleType::FUMAROLE_VERY_HOT);
    pVeryHot.Run();

    Pipeline::Pipeline pHot(files, Model::FumaroleType::FUMAROLE_HOT);
    pHot.Run();

    std::cout << "\nDone" << std::endl;

    return 0;
}

