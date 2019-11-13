//
// Segmentation.hpp
// Pipeline element that segments a thermal image based on k-means
//

#ifndef FUMAROLE_LOCALIZATION_SEGMENTATION_HPP
#define FUMAROLE_LOCALIZATION_SEGMENTATION_HPP

#include "pipeline/PipelineElement.hpp"

#include <string>
#include <vector>
#include <opencv2/core/types.hpp>

namespace Pipeline
{
    class Segmentation : public PipelineElement
    {
    public:
        /// Constructor
        /// \param name The name for this pipeline element
        explicit Segmentation(const std::string& name);

        ~Segmentation();

        /// Process the given thermal image into k segments
        /// \param input The input image
        /// \param output The segmented output image (quantized by k clusters in intensity)
        /// \param previousElementResult A reference to the result of the previous element. Expected to be an int for the value of k.
        /// \param result Will be set to nullptr as there is no result
        /// \param filename The name of the image file that is being processed
        virtual void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") override;

    private:
        void ConvertImageToPoints(const cv::Mat& image, std::vector<cv::Point2f>& points) const;
        void ConvertPointsToImage(const std::vector<cv::Point2f>& points, cv::Mat& image, int rows, int cols) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_SEGMENTATION_HPP
