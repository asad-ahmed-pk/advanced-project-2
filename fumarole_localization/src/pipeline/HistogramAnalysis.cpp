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
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace Pipeline
{
    const float MIN_REL_FREQUENCY_REQ = 0.1;

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

        // normalize by largest value to get relative frequencies
        auto maxFrequencyIter = std::max_element(m_BinFrequencies.begin(), m_BinFrequencies.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
            return a.second < b.second;
        });
        float maxFrequency = maxFrequencyIter->second > 0 ? maxFrequencyIter->second : 1.0;

        for (auto& kv : m_BinFrequencies) {
            kv.second /= maxFrequency;
        }

        // determine best value of k for k-means
        auto k = std::make_shared<int>(1);

        for (const auto& kv : m_BinFrequencies)
        {
            std::cout << "\n" << kv.first << ": " << kv.second;
            if (kv.second > MIN_REL_FREQUENCY_REQ) {
                *k += 1;
            }
        }
        std::cout << std::endl;

        // set result (k) and output is input as no processing done on image itself
        result = k;
        output = input;

        std::cout << "\nK for " << filename << ": " << *k << std::endl;
    }
}
