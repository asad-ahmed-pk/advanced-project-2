//
// Segmentation.cpp
// Pipeline element that segments a thermal image based on k-means
//

#include "pipeline/Segmentation.hpp"

#include <memory>

namespace Pipeline
{
    Segmentation::Segmentation(const std::string &name) : PipelineElement(name)
    {

    }

    Segmentation::~Segmentation() = default;

    // Segment thermal image
    void Segmentation::Process(const cv::Mat &input, cv::Mat &output,
                               const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result,
                               const std::string &filename)
    {
        // extract k from previous element's result
        auto kp = std::static_pointer_cast<int>(previousElementResult);

        // convert image to points for segmentation
        std::vector<cv::Point2f> points;
        std::shared_ptr<std::vector<cv::Point2f>> centers = std::make_shared<std::vector<cv::Point2f>>();
        cv::Mat labels;
        ConvertImageToPoints(input, points);

        cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER, 100, 0.0);
        cv::kmeans(points, *kp, labels, criteria, 100, 0, *centers);

        result = centers;

        // convert segmented points back to image (intensity quantization)
        ConvertPointsToImage(*centers, output, input.rows, input.cols);
        SaveResult(output, filename);
    }

    // Convert image to points
    void Segmentation::ConvertImageToPoints(const cv::Mat& image, std::vector<cv::Point2f> &points) const
    {
        for (int row = 0; row < image.rows; row++) {
            for (int col = 0; col < image.cols; col++) {
                points.emplace_back(cv::Point(image.at<uchar>(row, col), 1));
            }
        }
    }

    // Convert points to image
    void Segmentation::ConvertPointsToImage(const std::vector<cv::Point2f> &points, cv::Mat &image, int rows, int cols) const
    {
        image = cv::Mat(rows, cols, CV_8U);
        int r = 0, c = 0;
        for (int i = 0; i < points.size(); i++)
        {
            image.at<int>(r, c) = points[i].x;
            if ((c + 1) % cols == 0) {
                r++;
                c = 0;
            }
        }
    }
}
