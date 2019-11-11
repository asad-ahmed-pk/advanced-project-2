//
// Segmentation.hpp
// Pipeline element that segments a thermal image based on k-means
//

#ifndef FUMAROLE_LOCALIZATION_SEGMENTATION_HPP
#define FUMAROLE_LOCALIZATION_SEGMENTATION_HPP

#include "pipeline/PipelineElement.hpp"

#include <string>

namespace Pipeline
{
    class Segmentation : PipelineElement
    {
    public:
        /// Constructor
        /// \param name The name for this pipeline element
        explicit Segmentation(const std::string& name);

        ~Segmentation() override;

        /// Process the given thermal image into k segments
        /// \param input The input image
        /// \param output The segmented output image (quantized by k clusters in intensity)
        /// \param previousElementResult A reference to the result of the previous element. Expected to be an int for the value of k.
        /// \param result Will be set to nullptr as there is no result
        /// \param filename The name of the image file that is being processed
        virtual void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;
    };
}

#endif //FUMAROLE_LOCALIZATION_SEGMENTATION_HPP
