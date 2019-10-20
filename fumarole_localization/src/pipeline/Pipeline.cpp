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
#include "pipeline/FumaroleContour.hpp"
#include "pipeline/FumaroleLocalizer.hpp"
#include "config/ConfigParser.hpp"

namespace Pipeline
{
    // Constructor that runs on multiple images
    Pipeline::Pipeline(const std::map<std::string, std::string>& files, Model::FumaroleType type) : m_Files(files), m_FumaroleType(type)
    {
        // threshold values
        int t1 = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_very_hot");
        int t2 = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_hot");
        int t2Max = Config::ConfigParser::GetInstance().GetValue<int>("config.pipeline.heat_threshold.threshold_intensity_hot_upper");

        // type as string
        std::string typeName;

        // create the starting element (threshold) based on the fumarole type
        switch (m_FumaroleType)
        {
            // very hot
            case Model::FumaroleType::FUMAROLE_VERY_HOT: {
                auto h1 = std::make_unique<HeatThreshold>("very_hot_threshold", t1, 255);
                m_Elements.emplace_back(std::move(h1));
                typeName = "very_hot";
                break;
            }

            // hot
            case Model::FumaroleType::FUMAROLE_HOT: {
                auto h2 = std::make_unique<HeatThreshold>("hot_threshold", t2, t2Max);
                m_Elements.emplace_back(std::move(h2));
                typeName = "hot";
                break;
            }
        }

        // contour detection
        auto c1 =  std::make_unique<FumaroleContour>("contour_detection_" + typeName);
        m_Elements.emplace_back(std::move(c1));

        // final element - localize contours onto original image
        auto l1 = std::make_unique<FumaroleLocalizer>("localized_" + typeName);
        m_Elements.emplace_back(std::move(l1));
    }

    // Destructor
    Pipeline::~Pipeline() {}

    // Processing the pipeline
    cv::Mat input;
    cv::Mat output;

    std::shared_ptr<void> result;
    std::shared_ptr<void> previousResult;

    bool Pipeline::Run() const
    {
        for (const auto& file : m_Files)
        {
            std::cout << "\nProcessing " << file.first;

            // read in image
            input = std::move(cv::imread(file.second, cv::IMREAD_GRAYSCALE));
            if (!input.data) {
                return false;
            }

            for (const auto & m_Element : m_Elements)
            {
                // pass to each element in the pipeline
                m_Element->Process(input, output, previousResult, result, file.first);
                input = output;
                previousResult = result;
            }

            previousResult = nullptr;
        }

        return true;
    }
}
