//
// FumaroleDetection.hpp
// Encapsulates a result of a detected fumarole in an image
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLEDETECTION_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLEDETECTION_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include "model/FumaroleType.hpp"

namespace Detection
{
    struct FumaroleDetection
    {
        Model::FumaroleType Type;
        cv::Rect BoundingBox;
        std::vector<cv::Point> Contour;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLEDETECTION_HPP
