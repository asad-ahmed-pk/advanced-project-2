//
// Pipeline.cpp
// Main image processing pipeline to run on the images for detection
//


#include <memory>
#include <utility>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "pipeline/Pipeline.hpp"
#include "pipeline/HeatThreshold.hpp"
#include "pipeline/HistogramAnalysis.hpp"
#include "pipeline/FumaroleContour.hpp"
#include "pipeline/FumaroleLocalizer.hpp"
#include "config/ConfigParser.hpp"

namespace Pipeline
{
    const int HEAT_THRESHOLD_LOWER { 80 };
    const int HEAT_THRESHOLD_UPPER { 255 };

    // Constructor that runs on multiple images
    Pipeline::Pipeline(const std::map<std::string, std::string>& files) : m_Files(files)
    {
        // threshold values
        /*
        int t1 = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_very_hot");
        int t2 = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_hot");
        int t2Max = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_hot_upper");
        */

        // 1. Heat threshold - remove cold temperature range from thermal
        auto heat = std::make_unique<HeatThreshold>("heat_threshold", HEAT_THRESHOLD_LOWER, HEAT_THRESHOLD_UPPER);
        m_Elements.emplace_back(std::move(heat));

        // 2. Histogram analysis - find best value of k for heat bins for k-means segmentation
        auto histogram = std::make_unique<HistogramAnalysis>("histogram_analysis");
        m_Elements.emplace_back(std::move(histogram));

        /*
        // contour detection
        auto c1 =  std::make_unique<FumaroleContour>("contour_detection_" + typeName);
        m_Elements.emplace_back(std::move(c1));

        // final element - localize contours onto original image
        auto l1 = std::make_unique<FumaroleLocalizer>("localized_" + typeName);
        m_Elements.emplace_back(std::move(l1));
        */
    }

    // Destructor
    Pipeline::~Pipeline() {}

    // Processing the pipeline
    cv::Mat input;
    cv::Mat output;

    std::shared_ptr<void> result;
    std::shared_ptr<void> previousResult;

    bool Pipeline::Run()
    {
        for (const auto& file : m_Files)
        {
            std::cout << "\nProcessing " << file.first;

            // read in image
            input = std::move(cv::imread(file.second, cv::IMREAD_GRAYSCALE));
            if (!input.data) {
                std::cerr << "\nFailed to read file: " << file.second << std::endl;
                return false;
            }

            for (const auto & m_Element : m_Elements)
            {
                // pass to each element in the pipeline
                m_Element->Process(input, output, previousResult, result, file.first);
                input = output;
                previousResult = result;
            }

            // last element in the pipeline is the localization - save its input
            auto contours = std::static_pointer_cast<std::vector<std::vector<cv::Point>>>(result);
            m_Localizations[file.first] = std::move(*contours);

            previousResult = nullptr;
        }

        return true;
    }

    // Get final localizations
    PipelineLocalizations Pipeline::GetLocalizations() const {
        return m_Localizations;
    }
}
