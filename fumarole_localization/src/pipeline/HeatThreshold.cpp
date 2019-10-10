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
    HeatThreshold::HeatThreshold(const std::string &name) : PipelineElement(name)
    {
        m_ThresholdParam = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity");
    }

    // Process
    void HeatThreshold::Process(const cv::Mat& input, const std::string& filename) const
    {
        cv::Mat output;
        cv::threshold(input, output, m_ThresholdParam, 255.0, cv::ThresholdTypes::THRESH_TOZERO);

        if (!filename.empty()) {
            SaveResult(output, filename);
        }
    }
}
