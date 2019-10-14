//
// fumarole_data_io.hpp
// Contains functions for loading fumarole data from disk
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLE_DATA_IO_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLE_DATA_IO_HPP

#include <vector>
#include <map>
#include <string>

#include "config/config.hpp"

namespace IO
{
    /// Reads in the data item id and the file paths for the grey scale heat maps
    /// \param files A map with the file id and the file path that will be read in
    void GetGreyscaleHeatMaps(std::map<std::string, std::string>& files);

    /// Get the thermal image matching the file id
    /// \param fileID The file id of the thermal image
    /// \param image A reference to the cv::Mat that will be loaded
    /// \param readAsRGB If true, image will be read as RGB instead of grey scale
    /// \return Returns true if loaded successfully
    bool GetThermalImage(const std::string& fileID, cv::Mat& image, bool readAsRGB);
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLE_DATA_IO_HPP
