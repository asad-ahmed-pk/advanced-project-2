//
// Pipeline.hpp
// Main image processing pipeline to run on the images for detection
//

#ifndef FUMAROLE_LOCALIZATION_PIPELINE_HPP
#define FUMAROLE_LOCALIZATION_PIPELINE_HPP

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "pipeline/PipelineElement.hpp"
#include "model/FumaroleType.hpp"

namespace Pipeline
{
    // Typedef for the final localization result
    typedef std::map<std::string, std::vector<std::vector<cv::Point>>> PipelineLocalizations;

    class Pipeline
    {
    public:
        /// Create the default pipeline for this processing task
        /// \param files A map with the key as the file id (name) and the value the file path
        /// \return An instance of a pipeline with the given pipeline elements
        Pipeline(const std::map<std::string, std::string>& files);

        /// Destructor
        ~Pipeline();

        /// Run the pipeline with the configured elements
        bool Run();

        /// Get the final, processed localizations for each image that was run through this pipeline
        /// \return A copy of the processed localizations. The key in the map is the fileID, and the value is a list of contours.
        PipelineLocalizations GetLocalizations() const;

    private:
        std::map<std::string, std::string> m_Files;
        std::vector<std::unique_ptr<PipelineElement>> m_Elements;
        PipelineLocalizations m_Localizations;
    };
}

#endif //FUMAROLE_LOCALIZATION_PIPELINE_HPP
