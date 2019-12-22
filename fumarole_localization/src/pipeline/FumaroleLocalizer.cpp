//
// FumaroleLocalizer.cpp
// Localizes the fumaroles by taking input of the contour detection element
//

#include "pipeline/FumaroleLocalizer.hpp"
#include "pipeline/Typedefs.hpp"
#include "io/fumarole_data_io.hpp"

#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Pipeline
{
    // Constructor
    FumaroleLocalizer::FumaroleLocalizer(const std::string &name, bool saveResults) : PipelineElement(name, saveResults)
    {

    }

    FumaroleLocalizer::~FumaroleLocalizer()
    {

    }

    void FumaroleLocalizer::Process(const cv::Mat &input, cv::Mat &output, const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result, const std::string &filename)
    {
        // the merged contours that will be the final, localized fumaroles
        // after examining the contours from the different thermal ranges in the channels
        std::vector<std::vector<cv::Point>> contoursForImage;

        // get contours from previous pipeline element result
        auto contours = std::static_pointer_cast<FumaroleContours>(previousElementResult);

        // loop through the contours backwards (hot contours to cold)
        for (auto iter = contours->rbegin(); iter != contours->rend(); iter++) {
            std::copy_if(iter->begin(), iter->end(), std::back_inserter(contoursForImage), [&](const std::vector<cv::Point>& c) { return !IsContourEnclosingSomeContour(
                    c, contoursForImage); });
        }

        // draw the final contours onto the image
        if (m_SaveIntermediateResults)
        {
            //IO::GetThermalImage(filename, output, true);
            IO::GetFullResCamImage(filename, output);
            if (!contoursForImage.empty()) {
                cv::drawContours(output, contoursForImage, -1, cv::Scalar(0, 0, 255), 2);
            }
            SaveResult(output, filename);
        }

        // set result to vector of contours
        std::shared_ptr<std::vector<std::vector<cv::Point>>> contourList = std::make_shared<std::vector<std::vector<cv::Point>>>(contoursForImage);
        result = contourList;
    }

    // Return true if inner contour is inside outer contour
    bool FumaroleLocalizer::IsContourEnclosingContour(const std::vector<cv::Point> &outer,
                                                      const std::vector<cv::Point> &inner) const
    {
        // convert both to bounding boxes and check if inside
        cv::Rect innerRect = cv::boundingRect(inner);
        cv::Rect outerRect = cv::boundingRect(outer);

        return ((innerRect & outerRect) == innerRect);
    }

    // Returns true if the given contour is enclosing any one of the contours in contours
    bool FumaroleLocalizer::IsContourEnclosingSomeContour(const std::vector<cv::Point> &contour,
                                                          std::vector<std::vector<cv::Point>> &contours) const
    {
        for (const std::vector<cv::Point>& c : contours)
        {
            if (IsContourEnclosingContour(contour, c)) {
                return true;
            }
        }

        return false;
    }
}
