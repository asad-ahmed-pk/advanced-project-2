//
// FumaroleType.hpp
// Enum for representing the different classes of fumaroles
//

#ifndef FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP
#define FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP

#include <string>

namespace Model
{
    enum FumaroleType {
        FUMAROLE_HOLE,
        FUMAROLE_OPEN_VENT,
        FUMAROLE_HIDDEN,
        FUMAROLE_HEATED_AREA
    };

    // Convert enum type to string
    std::string TypeNameString(FumaroleType type);
}

#endif //FUMAROLE_LOCALIZATION_FUMAROLETYPE_HPP
