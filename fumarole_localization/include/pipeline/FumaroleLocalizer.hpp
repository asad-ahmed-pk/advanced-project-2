//
// FumaroleLocalizer.hpp
// Localizes the fumarole by taking input of the contour detection element
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP

#include "PipelineElement.hpp"

#include <string>

namespace Pipeline
{
    class FumaroleLocalizer : public PipelineElement
    {
    public:

        /// Constructor
        /// \param name The name of the pipeline element
        explicit FumaroleLocalizer(const std::string& name, bool saveResults);

        /// Destructor
        ~FumaroleLocalizer();

        /// Process the given input and produce the output
        /// \param input The input image
        /// \param output A reference to the output image
        /// \param previousElementResult A void pointer to arbitrary data of the previous pipeline element
        /// \param result A reference to the the pointer to set to the result for this pipeline element
        /// \param filename The name of the file that must be saved (if provided)
        void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        bool IsContourEnclosingContour(const std::vector<cv::Point>& inner, const std::vector<cv::Point>& outer) const;
        bool IsContourEnclosingSomeContour(const std::vector<cv::Point>& contour, std::vector<std::vector<cv::Point>>& contours) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP
