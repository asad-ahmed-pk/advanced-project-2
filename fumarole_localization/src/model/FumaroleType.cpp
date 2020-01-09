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

    cv::Scalar ColorForType(FumaroleType type)
    {
        switch (type)
        {
            case FumaroleType::FUMAROLE_HOLE:
                return cv::Scalar(10, 245, 206);

            case FumaroleType::FUMAROLE_HEATED_AREA:
                return cv::Scalar(40, 80, 230);

            case FumaroleType::FUMAROLE_OPEN_VENT:
                return cv::Scalar(255, 0, 0);

            case FumaroleType::FUMAROLE_HIDDEN_VENT:
                return cv::Scalar(222, 114, 47);

            case FumaroleType::UNKNOWN:
                return cv::Scalar(0, 0, 0);
        }
    }
}

