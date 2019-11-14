//
// HeatThreshold.cpp
// Performs a threshold for hot areas based on the config
//

#include "pipeline/HeatThreshold.hpp"
#include "config/ConfigParser.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Pipeline
{
    // Constructor
    HeatThreshold::HeatThreshold(const std::string &name) : PipelineElement(name)
    {
        // read in the heat ranges
        std::string bins = Config::ConfigParser::GetInstance().GetValue<std::string>("config.pipeline.histogram.bins");
        std::vector<std::string> binValues;
        boost::split(binValues, bins, boost::is_space());

        std::transform(binValues.begin(), binValues.end(), std::back_inserter(m_HeatRanges), [&](const std::string& str) { return std::stoi(str); });
    }

    // Process
    void HeatThreshold::Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename)
    {
        int lower = 0;
        int upper = 0;
        int c = 0;

        // set output image
        output = cv::Mat(input.rows, input.cols, CV_8UC3);

        // apply threshold and save in separate channel of output
        for (auto iter = m_HeatRanges.begin(); iter != m_HeatRanges.end(); iter++)
        {
            lower = *iter;
            auto nextIter = std::next(iter, 1);
            upper = (nextIter != m_HeatRanges.end() ? *nextIter : 256);

            ThresholdImage(input, output, c, lower, upper);

            c++;
        }

        // save intermediate results
        std::vector<cv::Mat> thresholds;
        cv::split(output, thresholds);
        c= 0;

        for (const cv::Mat& image : thresholds) {
            SaveResult(image, std::to_string(c++) + "_" + filename);
        }

        /*
        cv::threshold(input, output, m_Threshold, m_MaxValue, cv::ThresholdTypes::THRESH_TOZERO);
        //cv::inRange(input, m_Threshold, m_MaxValue, output);

        if (!filename.empty()) {
            SaveResult(output, filename);
        }
         */
    }

    // Apply threshold to input and store in given channel
    void HeatThreshold::ThresholdImage(const cv::Mat &input, cv::Mat &output, int channel, int lower, int upper)
    {
        cv::Mat thresholdOutput;
        cv::inRange(input, lower, upper, thresholdOutput);
        cv::Vec3b channels;

        for (int row = 0; row < output.rows; row++)
        {
            for (int col = 0; col < output.cols; col++)
            {
                channels = output.at<cv::Vec3b>(row, col);
                channels[channel] = thresholdOutput.at<uchar>(row, col);
                output.at<cv::Vec3b>(row, col) = channels;
            }
        }
    }
}
