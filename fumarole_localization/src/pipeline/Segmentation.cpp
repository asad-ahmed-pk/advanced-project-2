//
// Segmentation.cpp
// Pipeline element that segments a thermal image based on k-means
//

#include "pipeline/Segmentation.hpp"

namespace Pipeline
{
    Segmentation::Segmentation(const std::string &name) : PipelineElement(name)
    {

    }

    Segmentation::~Segmentation() = default;

    // Segment thermal image
    void Segmentation::Process(const cv::Mat &input, cv::Mat &output,
                               const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result,
                               const std::string &filename)
    {
        // TODO: segment using k-means

    }
}
