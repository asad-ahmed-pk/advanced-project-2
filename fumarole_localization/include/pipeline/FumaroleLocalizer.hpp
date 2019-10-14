//
// FumaroleLocalizer.hpp
// Localizes the fumarole by taking input of the contour detection element
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP

#include "pipeline/PipelineElement.hpp"

#include <string>

namespace Pipeline
{
    class FumaroleLocalizer : public Pipeline::PipelineElement
    {
        FumaroleLocalizer(const std::string& name);

        ~FumaroleLocalizer();

        void Process(const cv::Mat& input, cv::Mat& output, const std::shared_ptr<void>& previousElementResult, std::shared_ptr<void>& result, const std::string& filename = "") const override;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLELOCALIZER_HPP
