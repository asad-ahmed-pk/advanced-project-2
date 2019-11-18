//
// EigenUtils.cpp
// Utility class for converting between Eigen vectors and OpenCV
//

#include "utils/EigenUtils.hpp"

namespace Utils
{
    Eigen::Vector2f EigenUtils::CVPointToEigenVector(const cv::Point& point)
    {
        Eigen::Vector2f vector { point.x, point.y };
        return std::move(vector);
    }
}
