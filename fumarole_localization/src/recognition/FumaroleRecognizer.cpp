//
// FumaroleRecognizer.cpp
// Takes input for fumarole thermal images and recognizes fumaroles
//

#include "recognition/FumaroleRecognizer.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Recognition
{
    // Constructor
    FumaroleRecognizer::FumaroleRecognizer()
    {


    }

    // Destructor
    FumaroleRecognizer::~FumaroleRecognizer()
    {

    }

    // One-shot detection on single thermal image
    bool FumaroleRecognizer::RecognizeFumaroles(const std::string &thermalImagePath, Recognition::FumaroleDetectionResult &result) const
    {
        // load image from path
        cv::Mat image = cv::imread(thermalImagePath, cv::IMREAD_GRAYSCALE);

        // TODO: run pipeline on image

        return true;
    }
}
