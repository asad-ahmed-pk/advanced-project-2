//
// HistogramAnalysis.cpp
// Pipeline element that determines the heat intensity bins
// Using the histogram of the intensities in the thermal image
//
// Note: Only supports a greyscale thermal image
//

#include "pipeline/HistogramAnalysis.hpp"
#include "config/ConfigParser.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <iterator>
#include <boost/algorithm/string.hpp>

namespace Pipeline
{
    const float MIN_FREQUENCY_REQ = 10e6;

    HistogramAnalysis::HistogramAnalysis(const std::string &name) : PipelineElement(name)
    {
        // init the frequencies for the bins to 0 (read from config file)
        std::string bins = Config::ConfigParser::GetInstance().GetValue<std::string>("config.pipeline.histogram.bins");
        std::vector<std::string> binValues;

        boost::split(binValues, bins, boost::is_space());

        for (int i = 0; i < binValues.size(); i++) {
            int binNumber = std::stoi(binValues[i]);
            m_BinFrequencies[binNumber] = 0;
        }
    }

    HistogramAnalysis::~HistogramAnalysis() = default;

    // Process thresholded thermal image and find out best number of bins of heat
    void HistogramAnalysis::Process(const cv::Mat &input, cv::Mat &output,
                                    const std::shared_ptr<void> &previousElementResult, std::shared_ptr<void> &result,
                                    const std::string &filename)
    {
        cv::Mat image = input;
        cv::Mat histogram;

        int size = 256;
        float range[] { 0, 256 };
        const float* histRange = { range };

        // calculate histogram for all 256 values
        cv::calcHist(&image, 1, 0, cv::Mat(), histogram, 1, &size, &histRange);
        cv::normalize(histogram, histogram, 0, 256, cv::NORM_MINMAX, -1, cv::Mat());

        // count the frequencies of the bins setup in config
        for (std::map<int, float>::const_iterator iter = m_BinFrequencies.begin(); iter != m_BinFrequencies.end(); iter++)
        {
            auto nextIter = std::next(iter, 1);
            int upperBound = (nextIter != m_BinFrequencies.end() ? nextIter->first : 256);
            for (int row = iter->first; row < upperBound; row++) {
                int frequency = histogram.at<int>(row);
                m_BinFrequencies[iter->first] += frequency;
            }
        }

        // determine best value of k for k-means
        auto k = std::make_shared<int>(0);

        for (const auto& kv : m_BinFrequencies) {
            if (kv.second > MIN_FREQUENCY_REQ) {
                *k += 1;
            }
        }

        // set result (k) and output is input as no processing done on image itself
        result = k;
        output = input;

        std::cout << "\nK for " << filename << ": " << *k << std::endl;
    }
}
