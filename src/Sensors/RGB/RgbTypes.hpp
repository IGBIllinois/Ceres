
#pragma once

#include <string>

namespace rgb
{
    enum class eIMAGE_FORMAT
    {
        JPEG, MJPEG, H264, BITMAP
    };

    struct sImageSize_t
    {
        uint16_t width = 0;
        uint16_t height = 0;
    };

    inline bool operator==(const sImageSize_t& lhs, const sImageSize_t& rhs)
    {
        return (lhs.width == rhs.width) && (lhs.height == rhs.height);
    }

    inline bool operator!=(const sImageSize_t& lhs, const sImageSize_t& rhs)
    {
        return ! (lhs == rhs);
    }

} // end of axis namespace