
#pragma once

#include <string>

namespace axis
{
    enum class eIMAGE_FORMAT
    {
        JPEG, MJPEG, H264, BITMAP
    };

    std::string to_string(eIMAGE_FORMAT format);
    eIMAGE_FORMAT to_image_format(const std::string& str);


    struct sImageSize_t
    {
        uint16_t    width = 0;
        uint16_t    height = 0;
    };

    std::string to_string(sImageSize_t size);
    sImageSize_t to_image_size(const std::string& str);

} // end of axis namespace