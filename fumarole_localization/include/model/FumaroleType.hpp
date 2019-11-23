//
// FumaroleType.hpp
// Enum for representing the different classes of fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP

#include <string>
#include <vector>

namespace Model
{
    static std::vector<std::string> ClassLabels = {
        "fumarole_hole", "fumarole_open_vent", "fumarole_heated_area", "fumarole_hidden_vent"
    };

    enum FumaroleType {
        FUMAROLE_HOLE,
        FUMAROLE_OPEN_VENT,
        FUMAROLE_HIDDEN_VENT,
        FUMAROLE_HEATED_AREA,
        UNKNOWN
    };

    // Convert enum type to string
    std::string TypeNameString(FumaroleType type);
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP
