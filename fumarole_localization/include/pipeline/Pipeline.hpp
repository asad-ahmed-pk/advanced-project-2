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
    class Pipeline
    {
    public:
        /// Create the default pipeline for this processing task
        /// \param files A map with the key as the file id (name) and the value the file path
        /// \param type The fumarole type this pipeline is operating on and trying to extract
        /// \return An instance of a pipeline with the given pipeline elements
        Pipeline(const std::map<std::string, std::string>& files, Model::FumaroleType type);

        /// Destructor
        ~Pipeline();

        /// Run the pipeline with the configured elements
        bool Run() const;

    private:
        std::map<std::string, std::string> m_Files;
        std::vector<std::unique_ptr<PipelineElement>> m_Elements;
        Model::FumaroleType m_FumaroleType;
    };
}

#endif //FUMAROLE_LOCALIZATION_PIPELINE_HPP
