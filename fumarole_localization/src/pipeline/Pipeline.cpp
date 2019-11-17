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

namespace Pipeline
{
    // Constructor that runs on multiple images
    Pipeline::Pipeline(const std::map<std::string, std::string>& files) : m_Files(files)
    {
        // 1. Heat threshold - remove cold temperature range from thermal
        std::unique_ptr<HeatThreshold> heat = std::make_unique<HeatThreshold>("heat_threshold");
        m_Elements.emplace_back(std::move(heat));

        // 2. Contour detection - detect all contours present in the segmented image
        std::unique_ptr<FumaroleContour> contour = std::make_unique<FumaroleContour>("contours");
        m_Elements.emplace_back(std::move(contour));

        // 3. Localize all contours to outline fumaroles
        std::unique_ptr<FumaroleLocalizer> localizer = std::make_unique<FumaroleLocalizer>("localization");
        m_Elements.emplace_back(std::move(localizer));
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
            //auto contours = std::static_pointer_cast<FumaroleContours>(result);
            //m_Localizations[file.first] = std::move(*contours);

            previousResult = nullptr;
        }

        return true;
    }

    // Get final localizations
    PipelineLocalizations Pipeline::GetLocalizations() const {
        return m_Localizations;
    }
}
