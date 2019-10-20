//
// FumaroleRecognizer.hpp
// Takes input for fumarole thermal images and recognizes fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP

#include <map>
#include <memory>
#include <string>

#include "recognition/FumaroleDetectionResult.hpp"
#include "pipeline/Pipeline.hpp"

namespace Recognition
{
    // Typedef for this recognizers main output - a map where key:fileID, value: list of detections
    typedef std::map<std::string, std::vector<FumaroleDetectionResult>> DetectionMap;

    class FumaroleRecognizer
    {
    public:
        /// Default constructor
        FumaroleRecognizer();

        /// Destructor
        ~FumaroleRecognizer();

        /// Recognize all the fumaroles in the given image
        /// \param fileID The ID to associate with this image
        /// \param thermalImagePath The file path to the thermal image to load
        /// \param result A reference to the detection result that will be set
        /// \return Returns true on success
        bool RecognizeFumaroles(const std::string& fileID, const std::string& thermalImagePath, std::vector<Recognition::FumaroleDetectionResult> &results) const;

        /// Recognize all the fumaroles in the given image set and map bounding boxes for all of them
        /// \param files A map where key = the file id, and value = the file path to the thermal image
        /// \param results A reference that will be set to a map with key = file id, and value = a vector of detections for the fumaroles
        /// \return Returns true on success
        bool RecognizeFumaroles(const std::map<std::string, std::string>& files, std::map<std::string, std::vector<FumaroleDetectionResult>>& results) const;

    private:
        std::map<std::string, std::vector<FumaroleDetectionResult>> ConvertLocalizations(const Pipeline::PipelineLocalizations& localizations) const;
        DetectionMap MergeResults(const DetectionMap& m1, const DetectionMap& m2) const;
    };
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLERECOGNIZER_HPP
