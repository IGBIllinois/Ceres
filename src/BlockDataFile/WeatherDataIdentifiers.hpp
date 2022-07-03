
#pragma once

namespace weather
{
    /**********************************************************
       WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

       Only add new one to the end of the list.
    **********************************************************/
    enum class DataID : uint16_t
    {
        CONFIGURATION_INFO,
        WIND_DATA_VALID,
        WIND_SPEED_MPS,
        WIND_SPEED_KNOTS,
        WIND_DIRECTION_DEG,
    };
}


