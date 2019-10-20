//
// DatasetLoader.hpp
// Loads data from the file system
//

#ifndef FUMAROLE_LOCALIZATION_DATASETLOADER_HPP
#define FUMAROLE_LOCALIZATION_DATASETLOADER_HPP

#include <string>
#include <vector>
#include <map>
#include <opencv2/core/types.hpp>

namespace IO
{
    class DatasetLoader
    {
    public:
        /// Loads the file IDs from the given file
        /// \param fileIDs A reference to a vector that will be populated with the fileIDs
        static void GetTestFileIDs(std::vector<std::string> &fileIDs);

        /// Load the detections from the labelled image dir (expected to be labelimg XML files that match the fileIDs
        /// \param detections Will be loaded with the matching bounding boxes
        static void LoadTestData(std::map<std::string, std::vector<cv::Rect>>& detections);
    };
}

#endif //FUMAROLE_LOCALIZATION_DATASETLOADER_HPP
