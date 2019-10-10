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
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLE_DATA_IO_HPP
