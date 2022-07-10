
#pragma once

namespace ssnx
{
    /**********************************************************
       WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

       Only add new one to the end of the list.
    **********************************************************/
    enum class DataID : uint16_t
    {
        PVT_CARTESIAN,
        PVT_GEODETIC,
        POS_COV_GEODETIC,
        VEL_COV_GEODETIC,
        DOP,
        PVT_RESIDUALS,
        RAIM_STATISTICS,
        PVT_GEODETIC_AUTH,
        POS_PROJECTED,
        RECEIVER_TIME,
        RTCM_DATUM,
    };
}


