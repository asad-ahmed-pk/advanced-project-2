//
// FumaroleContour.cpp
// Pipeline element for detecting and drawing contours for fumaroles
// Expects input to be a threshold binary image
//

#include "pipeline/Typedefs.hpp"
#include "pipeline/FumaroleContour.hpp"
#include "config/ConfigParser.hpp"
#include "io/fumarole_data_io.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Pipeline
{
    // Constructor
    FumaroleContour::FumaroleContour(const std::string &name) : PipelineElement(name)
    {
        // read in min area param
        m_MinAreaFilter = Config::ConfigParser::GetInstance().GetValue<float>("config.pipeline.contour.min_area");
    }

    // Destructor
    FumaroleContour::~FumaroleContour() {}

    // Processing
    void FumaroleContour::Process(const cv::Mat &input, cv::Mat &output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string &filename)
    {
        auto contours = std::make_shared<FumaroleContours>();

        // split into channels - each channel is a separate heat thresholded image
        std::vector<cv::Mat> images;
        cv::split(input, images);
        for (int i = 0; i < images.size(); i++) {
            contours->emplace_back(FindContours(images[i]));
        }

        // set the result of the processing (contours)
        result = contours;

        // Save contour results
        SaveContourResults(*contours, filename);
    }

    // Finds contours in the given image
    std::vector<std::vector<cv::Point>> FumaroleContour::FindContours(const cv::Mat& image) const
    {
        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point>> contours;

        cv::findContours(image, contours, hierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

        // filter out noise (contours with very small areas)
        if (!contours.empty()) {
            FilterContourNoise(contours);
        }

        return std::move(contours);
    }

    // Remove the contours that are detected as noise (small area)
    void FumaroleContour::FilterContourNoise(std::vector<std::vector<cv::Point>>& contours) const
    {
        auto iter = contours.begin();
        while (iter != contours.end())
        {
            if (cv::contourArea(*iter) <= m_MinAreaFilter) {
                iter = contours.erase(iter);
            }
            else {
                iter++;
            }
        }
    }

    // Save intermediate contour results
    void FumaroleContour::SaveContourResults(const FumaroleContours& contours, const std::string& filename) const
    {
        int i = 0;
        for (const std::vector<std::vector<cv::Point>>& contoursForThermalRange : contours)
        {
            cv::Mat output;
            IO::GetThermalImage(filename, output, true);

            if (!contoursForThermalRange.empty()) {
                cv::drawContours(output, contoursForThermalRange, -1, cv::Scalar(0, 0, 255));
            }

            SaveResult(output, std::to_string(i++) + "_" + filename);
        }
    }
}
