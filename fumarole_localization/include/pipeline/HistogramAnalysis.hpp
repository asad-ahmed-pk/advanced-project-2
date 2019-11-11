//
// HistogramAnalysis.hpp
// Pipeline element that determines the heat intensity bins
// Using the histogram of the intensities in the thermal image
//
// Note: Only supports a greyscale thermal image
//

#ifndef FUMAROLE_LOCALIZATION_HISTOGRAMANALYSIS_HPP
#define FUMAROLE_LOCALIZATION_HISTOGRAMANALYSIS_HPP

#include <string>
#include <map>

#include "pipeline/PipelineElement.hpp"

namespace Pipeline
{
    class HistogramAnalysis : public PipelineElement
    {
    public:
        /// Constructor
        /// \param name The name of the pipeline element
        HistogramAnalysis(const std::string& name);

        ~HistogramAnalysis();

        /// Process the thermal image and determine the number of bins for the thermal ranges
        /// \param input The thermal image with only hot areas that could be potential fumaroles
        /// \param output A reference to the thermal image (this element does not produce any new output)
        /// \param previousElementResult The results of the previous element's output (other data)
        /// \param result Will be set to the number of heat 'bins' present in the thermal image
        /// \param filename The name of the file this element is processing
        virtual void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        std::map<int, float> m_BinFrequencies;
    };
}

#endif //FUMAROLE_LOCALIZATION_HISTOGRAMANALYSIS_HPP
