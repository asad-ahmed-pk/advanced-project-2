//
// HeatThreshold.hpp
// Performs a threshold for hot areas based on the config
// Outputs the thresholded images in N channels
//

#ifndef FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP
#define FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP

#include "pipeline/PipelineElement.hpp"

#include <vector>
#include <memory>

namespace Pipeline
{
    class HeatThreshold : public PipelineElement
    {
    public:
        HeatThreshold(const std::string &name);
        ~HeatThreshold() = default;

        /// Applies N threshold ranges (defined in the config file), and saves it in separate channels of the output image
        /// \param input The input to the threshold (greyscale image)
        /// \param output Will be a thresholded N-channel image
        /// \param previousElementResult A reference to the previous pipeline's result (expected to be null)
        /// \param result Not used
        /// \param filename The name of the file being processed
        void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        void ThresholdImage(const cv::Mat& input, cv::Mat& output, int channel, int lower, int upper);

    private:
        std::vector<int> m_HeatRanges;
    };
}

#endif //FUMAROLE_LOCALIZATION_HEATTHRESHOLD_HPP
