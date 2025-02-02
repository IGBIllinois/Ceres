
#pragma once

namespace  gps
{
    enum class eDatum
    {
        WGS84,
        DGNSS,
        ETRS89,
        NAD83,
        NAD83_PA11,
        NAD83_MA11,
        GDA94,
        FIRST_USER_DATUM,
        SECOND_USER_DATUM
    };

    enum class eSolutionType
    {
        NONE,
        STAND_ALONE,
        DIFFERENTIAL,
        FIXED,
        RTK_FIXED,
        RTK_FLOAT,
        SBAS,
        MOVING_RTK_FIXED,
        MOVING_RTK_FLOAT,
        PPP
    };

    struct sGpsPoint
    {
        double  Timestamp_s;
        double  Latitude_rad;
        double  Longitude_rad;
        double  Height_m;
        double  Vn_mps;
        double  Ve_mps;
        double  Vu_mps;
        double  GroundTrack_deg;
    };

    struct sPosition
    {
        double  Timestamp_s;
        double  Latitude_rad;
        double  Longitude_rad;
        double  Height_m;
    };

    struct sVelocity
    {
        double  Timestamp_s;
        double  Vn_mps;
        double  Ve_mps;
        double  Vu_mps;
    };

    struct sUtcDate
    {
        double  Timestamp_s;
        int  utcDay;
        int  utcMonth;
        int  utcYear;
    };

    struct sUtcTime
    {
        double  Timestamp_s;
        int  utcHour;
        int  utcMinute;
        int  utcSecond;
    };

    struct sReferencePosition
    {
        bool valid = false;
        double  Timestamp_s = 0;
        double  avgLatitude_rad = 0;
        double  avgLongitude_rad = 0;
        double  avgHeight_m = 0;
        double  stdLatitude_rad = 0;
        double  stdLongitude_rad = 0;
        double  stdHeight_m = 0;
        bool    heightValid = false;
    };

};

