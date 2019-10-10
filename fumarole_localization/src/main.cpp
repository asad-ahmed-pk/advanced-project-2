// main.cpp
// Main executable

#include <string>
#include <iostream>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "pipeline/HeatThreshold.hpp"
#include "io/fumarole_data_io.hpp"

int main(int argc, char** argv)
{

    // test one image
    cv::Mat image = cv::imread("../data/CamThermal_vis_grey/CamThermal_000762914000.bmp", cv::IMREAD_GRAYSCALE);

    // view image before processing
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);

    // threshold
    Pipeline::HeatThreshold ht("heat_threshold");

    std::map<std::string, std::string> files;
    IO::GetGreyscaleHeatMaps(files);

    for (std::pair<std::string, std::string> pair : files)
    {
        std::cout << "Processing: " << pair.first << std::endl;

        cv::Mat image = cv::imread(pair.second, cv::IMREAD_GRAYSCALE);
        if (!image.data) {
            std::cout << "Unable to read image: " << pair.first << std::endl;
            return -1;
        }

        ht.Process(image, pair.first);
    }

    std::cout << "\nDone" << std::endl;
    return 0;
}

