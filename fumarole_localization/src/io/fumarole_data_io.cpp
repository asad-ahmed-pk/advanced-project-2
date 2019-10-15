//
// fumarole_data_io.cpp
// Contains functions for loading fumarole data from disk
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include "config/config.hpp"
#include "io/fumarole_data_io.hpp"

namespace IO
{
    // Read all greyscale data
    void GetGreyscaleHeatMaps(std::map<std::string, std::string>& files)
    {
        std::string filename;
        boost::filesystem::path path;
        boost::filesystem::directory_iterator end;

        for (boost::filesystem::directory_iterator itr(Config::HEATMAPS_GREY_DIR); itr != end; itr++)
        {
            path = std::move(boost::filesystem::path(*itr));

            // remove the Camthermal prefix and extract the filename
            filename = path.stem().string();
            size_t index = filename.find(Config::THERMAL_IMAGE_PREFIX);
            if (index != std::string::npos) {
                filename = filename.substr(Config::THERMAL_IMAGE_PREFIX.size(), filename.size() - Config::THERMAL_IMAGE_PREFIX.size());
            }

            files[filename] = path.string();
        }
    }

    // Get the thermal image for the given file id
    bool GetThermalImage(const std::string& fileID, cv::Mat& image, bool readAsRGB)
    {
        const std::string filePath { Config::HEATMAPS_GREY_DIR + Config::THERMAL_IMAGE_PREFIX + fileID + Config::THERMAL_IMAGE_FILE_EXT };
        image = cv::imread(filePath, readAsRGB ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
        return (image.data != nullptr);
    }

    // Get cam image
    bool GetFullResCamImage(const std::string fileID, cv::Mat& image)
    {
        const std::string filePath { Config::FULL_RES_IMAGE_LEFT_CAM_DIR + Config::FULL_RES_IMAGE_PREFIX + "_" + fileID + Config::FULL_RES_IMAGE_FILE_EXT };
        image = cv::imread(filePath, cv::IMREAD_COLOR);
        return (image.data != nullptr);
    }
}