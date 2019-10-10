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
        HeatThreshold(const std::string &name);

        void Process(const cv::Mat& input, const std::string& filename = "") const;

    private:
        int m_ThresholdParam;
    };
}

#endif //FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP
