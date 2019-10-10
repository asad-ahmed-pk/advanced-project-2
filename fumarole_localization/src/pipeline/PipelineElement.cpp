//
// PipelineElement.cpp
// Generic class for representing an element in the image processing pipeline
//

#include "pipeline/PipelineElement.hpp"
#include "config/config.hpp"

#include <opencv2/highgui/highgui.hpp>

namespace Pipeline
{
    // Constructor
    PipelineElement::PipelineElement(const std::string &name) : m_Name(name)
    {

    }

    PipelineElement::~PipelineElement()
    {

    }

    // Write image to disk
    void PipelineElement::SaveResult(const cv::Mat &output, const std::string& filename) const
    {
        cv::imwrite(Config::PIPELINE_OUTPUT_DIR + filename + Config::IMAGE_OUTPUT_EXT, output);
    }
}
