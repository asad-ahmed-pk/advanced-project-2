//
// FumaroleRecognizer.hpp
// Takes input for fumarole thermal images and recognizes fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP

#include <memory>
#include <string>

#include "FumaroleDetectionResult.hpp"
#include "pipeline/Pipeline.hpp"

namespace Recognition
{
    class FumaroleRecognizer
    {
    public:
        /// Default constructor
        FumaroleRecognizer();

        /// Destructor
        ~FumaroleRecognizer();

        /// Recognize all the fumaroles in the given image
        /// \param thermalImagePath The file path to the thermal image to load
        /// \param result A reference to the detection result that will be set
        /// \return Returns true on success
        bool RecognizeFumaroles(const std::string& thermalImagePath, FumaroleDetectionResult& result) const;


    private:
        std::unique_ptr<Pipeline::Pipeline> m_DetectionPipeline;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP
