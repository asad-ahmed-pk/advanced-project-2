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

        [[nodiscard]] cv::Point2f Center() const {
            return std::move(cv::Point2f{static_cast<float>(BoundingBox.x + BoundingBox.width / 2.0), static_cast<float>(BoundingBox.y + BoundingBox.height / 2.0)});
        }
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLEDETECTION_HPP
