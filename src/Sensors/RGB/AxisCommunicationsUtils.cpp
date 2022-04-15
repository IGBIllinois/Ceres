
#include "AxisCommunicationsUtils.hpp"

std::string axis::to_string(eIMAGE_FORMAT format)
{
    switch (format)
    {
    case eIMAGE_FORMAT::JPEG:
        return std::string("jpeg");

    case eIMAGE_FORMAT::MJPEG:
        return std::string("mjpeg");

    case eIMAGE_FORMAT::H264:
        return std::string("h264");

    case eIMAGE_FORMAT::BITMAP:
        return std::string("bitmap");
    }

    return std::string();
}


axis::eIMAGE_FORMAT axis::to_image_format(const std::string& str)
{
    if (str == "mjpeg")
        return eIMAGE_FORMAT::MJPEG;
    if (str == "h264")
        return eIMAGE_FORMAT::H264;
    if (str == "bitmap")
        return eIMAGE_FORMAT::BITMAP;

    return eIMAGE_FORMAT::JPEG;
}


std::string axis::to_string(sImageSize_t size)
{
    std::string result = std::to_string(size.width);
    result += "x";
    result += std::to_string(size.height);

    return result;
}

axis::sImageSize_t axis::to_image_size(const std::string& str)
{
    axis::sImageSize_t image_size;

    auto n = str.find('x');
    if (n != std::string::npos)
    {
        std::string width  = str.substr(0, n);
        std::string height = str.substr(n+1);
        image_size.width  = std::stoi(width);
        image_size.height = std::stoi(height);
    }

    return image_size;
}
