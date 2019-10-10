//
// fumarole_data_io.cpp
// Contains functions for loading fumarole data from disk
//

#include <iostream>

#include <boost/filesystem.hpp>

#include "config/config.hpp"
#include "io/fumarole_data_io.hpp"

namespace IO
{
    // Read all greyscale data
    void GetGreyscaleHeatMaps(std::map<std::string, std::string>& files)
    {
        boost::filesystem::path path;
        boost::filesystem::directory_iterator end;

        for (boost::filesystem::directory_iterator itr(Config::HEATMAPS_GREY_DIR); itr != end; itr++)
        {
            path = std::move(boost::filesystem::path(*itr));
            files[path.stem().string()] = path.string();
        }
    }
}