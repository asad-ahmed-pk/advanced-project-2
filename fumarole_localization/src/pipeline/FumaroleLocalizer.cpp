//
// FumaroleLocalizer.cpp
// Localizes the fumaroles by taking input of the contour detection element
//

#include "pipeline/FumaroleLocalizer.hpp"
#include "pipeline/Typedefs.hpp"

#include <vector>
#include <set>
#include <opencv2/core/core.hpp>

namespace Pipeline
{
    // Constructor
    FumaroleLocalizer::FumaroleLocalizer(const std::string &name) : PipelineElement(name)
    {

    }

    FumaroleLocalizer::~FumaroleLocalizer()
    {

    }

    void FumaroleLocalizer::Process(const cv::Mat &input, cv::Mat &output, const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result, const std::string &filename)
    {
        // get contours from previous pipeline element result
        auto contours = std::static_pointer_cast<FumaroleContours>(previousElementResult);

        // loop through the contours backwards (hot contours to cold)
        for (auto iter = contours->rbegin(); iter != contours->rend(); iter++)
        {



        }
    }
}
