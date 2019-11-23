//
// FumaroleType.hpp
// Enum for representing the different classes of fumaroles
//

#include "model/FumaroleType.hpp"
#include <string>

namespace Model
{
    std::string TypeNameString(FumaroleType type)
    {
        switch (type) {
            case FUMAROLE_HOLE:
                return "fumarole_hole";
            case FUMAROLE_OPEN_VENT:
                return "fumarole_open_vent";
            case FUMAROLE_HIDDEN_VENT:
                return "fumarole_hidden_vent";
            case FUMAROLE_HEATED_AREA:
                return "fumarole_heated_area";
            case UNKNOWN:
                return "unknown";
        }
    }
}

