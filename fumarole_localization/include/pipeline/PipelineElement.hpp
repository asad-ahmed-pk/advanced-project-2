//
// PipelineElement.hpp
// Generic class for representing an element in the image processing pipeline
//

#ifndef FUMAROLE_LOCALIZATION_PIPELINEELEMENT_H
#define FUMAROLE_LOCALIZATION_PIPELINEELEMENT_H

#include <opencv2/core.hpp>
#include <string>

namespace Pipeline
{
    class PipelineElement
    {
    public:
        /// Construct a pipeline element with an input
        /// \param name The name of this element to identify
        PipelineElement(const std::string &name);

        /// Destructor
        virtual ~PipelineElement();

        /// Perform the processing for this element
        /// \param input The input image to process
        /// \param filename If not an empty string - will be used to save the output of this processing
        /// Note: filename should not include the file extension
        virtual void Process(const cv::Mat& input, cv::Mat& output, const std::string& filename = "") const = 0;

    protected:
        void SaveResult(const cv::Mat& output, const std::string& filename) const;

    protected:
        std::string m_Name;
    };
}

#endif //FUMAROLE_LOCALIZATION_PIPELINEELEMENT_H
