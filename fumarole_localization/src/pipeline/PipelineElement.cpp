//
// PipelineElement.cpp
// Generic class for representing an element in the image processing pipeline
//

#include "pipeline/PipelineElement.hpp"
#include "config/config.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
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
        std::string elementPath { Config::PIPELINE_OUTPUT_DIR + m_Name + "/" };
        std::string fullPath =  elementPath + filename + Config::IMAGE_OUTPUT_EXT;

        boost::filesystem::path path(elementPath);
        if (!boost::filesystem::exists(elementPath)) {
            boost::filesystem::create_directories(elementPath);
        }

        cv::imwrite(fullPath, output);
    }
}
