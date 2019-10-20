//
// FumaroleDetectionResult.hpp
// Encapsulates a result of a detected fumarole in an image
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include "model/FumaroleType.hpp"

namespace Recognition
{
    struct FumaroleDetectionResult
    {
        std::string ImageID;
        Model::FumaroleType Type;
        cv::Rect BoundingBox;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLEDETECTIONRESULT_HPP
