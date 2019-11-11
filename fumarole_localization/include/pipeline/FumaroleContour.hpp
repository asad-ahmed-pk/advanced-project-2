//
// FumaroleContour.hpp
// Pipeline element for detecting and drawing contours for fumaroles
// Expects input to be a threshold binary image
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP

#include <opencv2/core/core.hpp>
#include <vector>
#include <string>

#include "PipelineElement.hpp"

namespace Pipeline
{
    class FumaroleContour : public PipelineElement
    {
    public:
        /// Constructor
        /// \param name The name of this contour pipeline processing element
        explicit FumaroleContour(const std::string& name);

        /// Destructor
        ~FumaroleContour();

        /// Process the input and produce output
        /// \param input The input image to process
        /// \param output A reference to the output image that will be set with the output
        /// \param filename The name of the file to use if intermediate results are to be written to file
        void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        void FilterContourNoise(std::vector<std::vector<cv::Point>>& contours) const;

    private:
        float m_MinAreaFilter;
    };
}


#endif //FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP
