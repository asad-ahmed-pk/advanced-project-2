//
// HeatThreshold.cpp
// Performs a threshold for hot areas based on the config
//

#include "pipeline/HeatThreshold.hpp"
#include "config/ConfigParser.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Pipeline
{
    // Constructor
    HeatThreshold::HeatThreshold(const std::string &name, int threshold, int maxValue) : PipelineElement(name), m_Threshold(threshold), m_MaxValue(maxValue)
    {

    }

    // Process
    void HeatThreshold::Process(const cv::Mat& input, cv::Mat& output, const std::string& filename) const
    {
        cv::threshold(input, output, m_Threshold, m_MaxValue, cv::ThresholdTypes::THRESH_BINARY);

        if (!filename.empty()) {
            SaveResult(output, filename);
        }
    }
}
