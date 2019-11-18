//
// EigenUtils.hpp
// Utility class for converting between Eigen vectors and OpenCV
//

#ifndef FUMAROLE_LOCALIZATION_EIGENUTILS_HPP
#define FUMAROLE_LOCALIZATION_EIGENUTILS_HPP

#include <opencv2/core/core.hpp>
#include <Eigen/Eigen>

namespace Utils
{
    class EigenUtils
    {
    public:
        /// Convert the given OpenCV point to Eigen vector
        static Eigen::Vector2f CVPointToEigenVector(const cv::Point& point);
    };
}

#endif //FUMAROLE_LOCALIZATION_EIGENUTILS_HPP
