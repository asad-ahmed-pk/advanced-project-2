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

    const std::string THERMAL_IMAGE_PREFIX { "" };
    const std::string THERMAL_IMAGE_FILE_EXT { ".bmp" };
    const std::string THERMAL_IMAGES_DIR {"../data/thermal/"};

    const std::string FULL_RES_IMAGE_PREFIX { "CamLeft" };
    const std::string FULL_RES_IMAGE_FILE_EXT { ".tiff" };
    const std::string FULL_RES_IMAGE_LEFT_CAM_DIR { "../data/camera/" };

    const std::string RESOURCES_DIR { "../resources/" };
    const std::string TEST_ID_LIST_FILE_NAME { "fumarole_test_list.txt" };

    const std::string FINAL_RESULTS_OUTPUT_DIR { "results/" };
    const std::string EVALUATION_IMAGES_OUTPUT_DIR {"evaluation/" };
}

#endif //FUMAROLE_LOCALIZATION_CONFIG_HPP
