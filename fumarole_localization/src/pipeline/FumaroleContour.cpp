//
// FumaroleContour.cpp
// Pipeline element for detecting and drawing contours for fumaroles
// Expects input to be a threshold binary image
//

#include "pipeline/FumaroleContour.hpp"
#include "config/ConfigParser.hpp"

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
    void FumaroleContour::Process(const cv::Mat &input, cv::Mat &output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string &filename) const
    {
        auto contourPoints = std::make_shared<std::vector<std::vector<cv::Point>>>();
        std::vector<cv::Vec4i> hierarchy;

        output = cv::Mat::zeros(input.rows, input.cols, CV_8UC3);

        cv::findContours(input, *contourPoints, hierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

        // filter out noise (contours with very small areas)
        if (!contourPoints->empty())
        {
            FilterContourNoise(*contourPoints);

            if (!contourPoints->empty())
            {
                /*
                for (const auto &c : *contourPoints) {
                    std::cout << "Contour Area: " << cv::contourArea(c) << std::endl;
                }
                */

                // draw contours
                cv::drawContours(output, *contourPoints, -1, cv::Scalar(0, 0, 255), 1, cv::LINE_8);

                // save this result to disk if contours present
                if (!filename.empty()) {
                    SaveResult(output, filename);
                }


            }
        }

        // set the result of the processing (contours)
        result = contourPoints;
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
}
