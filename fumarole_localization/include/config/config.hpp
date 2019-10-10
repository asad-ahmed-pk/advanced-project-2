//
// config.hpp
// Contains config constants
//

#ifndef FUMAROLE_LOCALIZATION_CONFIG_HPP
#define FUMAROLE_LOCALIZATION_CONFIG_HPP

#include <string>

namespace Config
{
    const std::string CONFIG_FILE_PATH { "config/config.xml" };
    const std::string PIPELINE_OUTPUT_DIR { "pipeline_output/"};
    const std::string IMAGE_OUTPUT_EXT { ".png" };

    const std::string HEATMAPS_GREY_DIR { "../data/CamThermal_vis_grey/"};
}

#endif //FUMAROLE_LOCALIZATION_CONFIG_HPP
