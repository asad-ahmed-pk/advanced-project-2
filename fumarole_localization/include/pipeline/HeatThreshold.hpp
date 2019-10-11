//
// HeatThreshold.hpp
// Performs a threshold for hot areas based on the config
//

#ifndef FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP
#define FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP

#include "pipeline/PipelineElement.hpp"

namespace Pipeline
{
    class HeatThreshold : public PipelineElement
    {
    public:
        HeatThreshold(const std::string &name, int threshold, int maxValue);

        void Process(const cv::Mat& input, cv::Mat& output, const std::string& filename = "") const;

    private:
        int m_Threshold;
        int m_MaxValue;
    };
}

#endif //FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP
