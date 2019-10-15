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

    const std::string THERMAL_IMAGE_PREFIX { "CamThermal_" };
    const std::string THERMAL_IMAGE_FILE_EXT { ".bmp" };
    const std::string HEATMAPS_GREY_DIR { "../data/CamThermal_vis_grey/"};

    const std::string FULL_RES_IMAGE_PREFIX { "CamLeft" };
    const std::string FULL_RES_IMAGE_FILE_EXT { ".tiff" };
    const std::string FULL_RES_IMAGE_LEFT_CAM_DIR { "../data/CamLeft/" };
}

#endif //FUMAROLE_LOCALIZATION_CONFIG_HPP
