//
// Typedefs.hpp
// Contains common typedefs used throughout the pipeline
//

#ifndef FUMAROLE_LOCALIZATION_TYPEDEFS_HPP
#define FUMAROLE_LOCALIZATION_TYPEDEFS_HPP

#include <map>
#include <vector>
#include <opencv2/core/core.hpp>

// A vector where each element is:
//  a vector where each element is (a vector of contours):
//      a vector of points (a contour)
typedef std::vector<std::vector<std::vector<cv::Point>>> FumaroleContours;

#endif //FUMAROLE_LOCALIZATION_TYPEDEFS_HPP
