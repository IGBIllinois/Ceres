
#pragma once

#include "../RgbTypes.hpp"

#include <cbdf/ImageBuffers.hpp>

#include <string>

#include <QBitmap>
#include <QImage>


namespace lucid
{
    std::string to_string(rgb::eIMAGE_FORMAT format);
    rgb::eIMAGE_FORMAT to_image_format(const std::string& str);

    std::string to_string(rgb::sImageSize_t size);
    rgb::sImageSize_t to_image_size(const std::string& str);

    void to_buffer(const QBitmap& img, cBitmapBuffer& out);
    void to_buffer(const QImage& img, cJpegBuffer& out);
    void to_buffer(const QImage& img, cMpegFrameBuffer& out);

    void to_image(const cBitmapBuffer& in, QBitmap& out);
    void to_image(const cJpegBuffer& in, QImage& out);
    void to_image(const cMpegFrameBuffer& img, QImage& out);

} // end of axis namespace