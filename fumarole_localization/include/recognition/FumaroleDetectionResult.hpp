//
// FumaroleDetectionResult.hpp
// Encapsulates a result of running the detection algorithm on a single Fumarole thermal image
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP

#include <vector>
#include <opencv2/core/types.hpp>

namespace Recognition
{
    struct FumaroleDetectionResult
    {
        std::string ImageID;
        std::vector<cv::Rect> Detections;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP
