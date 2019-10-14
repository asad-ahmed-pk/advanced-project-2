//
// FumaroleLocalizer.cpp
// Localizes the fumarole by taking input of the contour detection element
//

#include "pipeline/FumaroleLocalizer.hpp"

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
        // draw the contours onto the main image




    }
}
