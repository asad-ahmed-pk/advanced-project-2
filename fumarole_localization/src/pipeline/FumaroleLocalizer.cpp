//
// FumaroleLocalizer.cpp
// Localizes the fumarole by taking input of the contour detection element
//

#include "pipeline/FumaroleLocalizer.hpp"
#include "io/fumarole_data_io.hpp"

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Pipeline
{
    // Constructor
    FumaroleLocalizer::FumaroleLocalizer(const std::string &name) : PipelineElement(name)
    {

    }

    FumaroleLocalizer::~FumaroleLocalizer()
    {

    }

    void FumaroleLocalizer::Process(const cv::Mat &input, cv::Mat &output, const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result, const std::string &filename) const
    {
        // get the original thermal image
        cv::Mat image;

        if (IO::GetThermalImage(filename, image, true))
        {
            // get the contours
            auto contours = std::static_pointer_cast<std::vector<std::vector<cv::Point>>>(previousElementResult);
            if (contours != nullptr)
            {
                cv::drawContours(image, *contours, -1, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
                SaveResult(image, filename);
            }
        }
    }
}
