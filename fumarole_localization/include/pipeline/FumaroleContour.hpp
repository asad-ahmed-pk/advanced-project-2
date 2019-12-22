//
// FumaroleContour.hpp
// Pipeline element for detecting and drawing contours for fumaroles
// Expects input to be a threshold binary image with each chanel representing a different threshold result
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP

#include "pipeline/Typedefs.hpp"
#include "pipeline/PipelineElement.hpp"

#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include <map>

namespace Pipeline
{
    class FumaroleContour : public PipelineElement
    {
    public:
        /// Constructor
        /// \param name The name of this contour pipeline processing element
        FumaroleContour(const std::string& name, bool saveResults);

        /// Destructor
        ~FumaroleContour();

        /// Process the input and produce output
        /// \param input The input image to process
        /// \param output A reference to the output image that will be set with the output
        /// \param filename The name of the file to use if intermediate results are to be written to file
        void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        void FilterContourNoise(std::vector<std::vector<cv::Point>>& contours) const;
        std::vector<std::vector<cv::Point>> FindContours(const cv::Mat& image) const;
        void SaveContourResults(const FumaroleContours& contours, const std::string& filename) const;

    private:
        float m_MinAreaFilter;
    };
}


#endif //FUMAROLE_LOCALIZATION_FUMAROLECONTOUR_HPP
