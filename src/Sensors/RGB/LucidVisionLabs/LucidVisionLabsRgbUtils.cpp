
#include "LucidVisionLabsRgbUtils.hpp"

#include "StringUtils.hpp"

#include <QImageWriter>
#include <QImageReader>
#include <QIODevice>

//#define CHECK_Q_IMAGE_READER

namespace
{
    class cImageWriteAdapter : public QIODevice
    {
    public:
        cImageWriteAdapter(cImageBuffer* byteArray)
            : mpData(byteArray)
        {
            QIODevice::open(QIODevice::WriteOnly | QIODevice::Unbuffered);
        }

        bool open(OpenMode flags) override
        {
            return false;
        }

        qint64 size() const override
        {
            return qint64(mpData->size());
        }

        bool seek(qint64 pos) override
        {
            if (pos > mpData->size() && isWritable())
            {
                if (seek(mpData->size()))
                {
                    const qint64 gapSize = pos - mpData->size();
                    if (write(QByteArray(gapSize, 0)) != gapSize) 
                    {
                        return false;
                    }
                }
                else 
                {
                    return false;
                }
            }
            else if (pos > mpData->size() || pos < 0)
            {
                return false;
            }

            return QIODevice::seek(pos);
        }

        bool canReadLine() const override
        {
            if (!isOpen())
                return false;

            return QIODevice::canReadLine();
        }

    protected:
        qint64	readData(char* data, qint64 len) override
        {
            if ((len = qMin(len, qint64(mpData->size()) - pos())) <= 0)
                return qint64(0);
            memcpy(data, mpData->data() + pos(), len);
            return len;
        }

        qint64	writeData(const char* data, qint64 len) override
        {
            int extraBytes = pos() + len - mpData->size();

            if (extraBytes > 0) // overflow
            {
                int newSize = mpData->size() + extraBytes;
                mpData->resize(newSize);
                if (mpData->size() != newSize) // could not resize
                { 
                    return -1;
                }
            }

            memcpy(mpData->data() + pos(), data, int(len));

            return len;
        }

    private:
        cImageBuffer* mpData;
    };


    class cImageReadAdapter : public QIODevice
    {
    public:
        cImageReadAdapter()
            : mpData(nullptr)
        {
            QIODevice::open(QIODevice::ReadOnly | QIODevice::Unbuffered);
        }

        cImageReadAdapter(const cImageBuffer* byteArray)
            : mpData( const_cast<cImageBuffer*>(byteArray) )
        {
            QIODevice::open(QIODevice::ReadOnly | QIODevice::Unbuffered);
        }

        void setImageBuffer(const cImageBuffer* byteArray)
        {
            mpData = const_cast<cImageBuffer*>(byteArray);
        }

        bool open(OpenMode flags) override
        {
            return false;
        }

        qint64 size() const override
        {
            return qint64(mpData->size());
        }

        bool seek(qint64 pos) override
        {
            if (pos > mpData->size() && isWritable())
            {
                if (seek(mpData->size()))
                {
                    const qint64 gapSize = pos - mpData->size();
                    if (write(QByteArray(gapSize, 0)) != gapSize)
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else if (pos > mpData->size() || pos < 0)
            {
                return false;
            }

            return QIODevice::seek(pos);
        }

        bool canReadLine() const override
        {
            if (!isOpen())
                return false;

            return QIODevice::canReadLine();
        }

    protected:
        qint64	readData(char* data, qint64 len) override
        {
            if ((len = qMin(len, qint64(mpData->size()) - pos())) <= 0)
                return qint64(0);
            memcpy(data, mpData->data() + pos(), len);
            return len;
        }

        qint64	writeData(const char* data, qint64 len) override
        {
            return qint64(0);
        }

    private:
        cImageBuffer* mpData;
    };
}

std::string lucid::to_string(rgb::eIMAGE_FORMAT format)
{
    switch (format)
    {
    case rgb::eIMAGE_FORMAT::JPEG:
        return std::string("jpeg");

    case rgb::eIMAGE_FORMAT::MJPEG:
        return std::string("mjpeg");

    case rgb::eIMAGE_FORMAT::H264:
        return std::string("h264");

    case rgb::eIMAGE_FORMAT::BITMAP:
        return std::string("bitmap");
    }

    return std::string();
}


rgb::eIMAGE_FORMAT lucid::to_image_format(const std::string& str)
{
    if (str == "mjpeg")
        return rgb::eIMAGE_FORMAT::MJPEG;
    if (str == "h264")
        return rgb::eIMAGE_FORMAT::H264;
    if (str == "bitmap")
        return rgb::eIMAGE_FORMAT::BITMAP;

    return rgb::eIMAGE_FORMAT::JPEG;
}


std::string lucid::to_string(rgb::sImageSize_t size)
{
    std::string result = std::to_string(size.width);
    result += "x";
    result += std::to_string(size.height);

    return result;
}

rgb::sImageSize_t lucid::to_image_size(const std::string& str)
{
    rgb::sImageSize_t image_size;

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

void lucid::to_buffer(const QBitmap& img, cBitmapBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "bmp");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img.toImage());
}

void lucid::to_buffer(const QImage& img, cJpegBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "jpeg");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img);
}

void lucid::to_buffer(const QImage& img, cMpegFrameBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "jpeg");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img);
}

void lucid::to_image(const cBitmapBuffer& in, QBitmap& out)
{
    cImageReadAdapter imageBuffer(&in);

    QImageReader  imageReader(&imageBuffer, "bmp");

    out.clear();
    imageBuffer.seek(0);

    QImage image;

#ifdef CHECK_Q_IMAGE_READER
    if (imageReader.read(&image))
        return;

    auto msg = imageReader.errorString().toStdString();
    throw std::runtime_error(msg);
#else
    imageReader.read(&image);
#endif

    out.fromImage(image);
}

void lucid::to_image(const cJpegBuffer& in, QImage& out)
{
    cImageReadAdapter imageBuffer(&in);

    QImageReader  imageReader(&imageBuffer, "jpeg");

    imageBuffer.seek(0);

#ifdef CHECK_Q_IMAGE_READER
    if (imageReader.read(&out))
        return;

    auto msg = imageReader.errorString().toStdString();
    throw std::runtime_error(msg);
#else
    imageReader.read(&out);
#endif
}

void lucid::to_image(const cMpegFrameBuffer& img, QImage& out)
{
    cImageReadAdapter imageBuffer(&img);

    QImageReader  imageReader(&imageBuffer, "jpeg");

    imageBuffer.seek(0);

#ifdef CHECK_Q_IMAGE_READER
    if (imageReader.read(&out))
        return;

    auto msg = imageReader.errorString().toStdString();
    throw std::runtime_error(msg);
#else
    imageReader.read(&out);
#endif
}


nLucidVisionLabsConnect::nTriton::ePixelFormat lucid::to_pixel_format(const std::string& pixel_format)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(pixel_format, "Mono8"))            return ePixelFormat::Mono8;
    if (nStringUtils::iequal(pixel_format, "Mono10"))           return ePixelFormat::Mono10;
    if (nStringUtils::iequal(pixel_format, "Mono10p"))          return ePixelFormat::Mono10p;
    if (nStringUtils::iequal(pixel_format, "Mono10Packed"))     return ePixelFormat::Mono10Packed;
    if (nStringUtils::iequal(pixel_format, "Mono12"))           return ePixelFormat::Mono12;
    if (nStringUtils::iequal(pixel_format, "Mono12p"))          return ePixelFormat::Mono12p;
    if (nStringUtils::iequal(pixel_format, "Mono12Packed"))     return ePixelFormat::Mono12Packed;
    if (nStringUtils::iequal(pixel_format, "Mono16"))           return ePixelFormat::Mono16;
    if (nStringUtils::iequal(pixel_format, "BayerRG8"))         return ePixelFormat::BayerRG8;
    if (nStringUtils::iequal(pixel_format, "BayerRG10"))        return ePixelFormat::BayerRG10;
    if (nStringUtils::iequal(pixel_format, "BayerRG10p"))       return ePixelFormat::BayerRG10p;
    if (nStringUtils::iequal(pixel_format, "BayerRG10Packed"))  return ePixelFormat::BayerRG10Packed;
    if (nStringUtils::iequal(pixel_format, "BayerRG12"))        return ePixelFormat::BayerRG12;
    if (nStringUtils::iequal(pixel_format, "BayerRG12p"))       return ePixelFormat::BayerRG12p;
    if (nStringUtils::iequal(pixel_format, "BayerRG12Packed"))  return ePixelFormat::BayerRG12Packed;
    if (nStringUtils::iequal(pixel_format, "BayerRG16"))        return ePixelFormat::BayerRG16;
    if (nStringUtils::iequal(pixel_format, "RGB8"))             return ePixelFormat::RGB8;
    if (nStringUtils::iequal(pixel_format, "BGR8"))             return ePixelFormat::BGR8;
    if (nStringUtils::iequal(pixel_format, "YCbCr8"))           return ePixelFormat::YCbCr8;
    if (nStringUtils::iequal(pixel_format, "YCbCr8_CbYCr"))     return ePixelFormat::YCbCr8_CbYCr;
    if (nStringUtils::iequal(pixel_format, "YUV422_8"))         return ePixelFormat::YUV422_8;
    if (nStringUtils::iequal(pixel_format, "YUV422_8_UYVY"))    return ePixelFormat::YUV422_8_UYVY;
    if (nStringUtils::iequal(pixel_format, "YCbCr411_8"))       return ePixelFormat::YCbCr411_8;
    if (nStringUtils::iequal(pixel_format, "YUV411_8_UYYVYY"))  return ePixelFormat::YUV411_8_UYYVYY;
    if (nStringUtils::iequal(pixel_format, "QOI_Mono8"))        return ePixelFormat::QOI_Mono8;
    if (nStringUtils::iequal(pixel_format, "QOI_BayerRG8"))     return ePixelFormat::QOI_BayerRG8;
    if (nStringUtils::iequal(pixel_format, "QOI_RGB8"))         return ePixelFormat::QOI_RGB8;
    if (nStringUtils::iequal(pixel_format, "QOI_BGR8"))         return ePixelFormat::QOI_BGR8;
    if (nStringUtils::iequal(pixel_format, "QOI_YCbCr8"))       return ePixelFormat::QOI_YCbCr8;
    if (nStringUtils::iequal(pixel_format, "QOI_YCbCr8_CbYC"))  return ePixelFormat::QOI_YCbCr8_CbYCr;

    return ePixelFormat::BayerRG16;
}

nLucidVisionLabsConnect::nTriton::eExposureAutoAlgorithm lucid::to_exposure_auto_algorithm(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "MEDIAN"))    return eExposureAutoAlgorithm::MEDIAN;
    if (nStringUtils::iequal(str, "MEAN"))      return eExposureAutoAlgorithm::MEAN;

    return eExposureAutoAlgorithm::MEAN;
}

nLucidVisionLabsConnect::nTriton::eExposureAutoLimitAuto lucid::to_exposure_auto_limit_auto(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "OFF"))        return eExposureAutoLimitAuto::OFF;
    if (nStringUtils::iequal(str, "CONTINUOUS")) return eExposureAutoLimitAuto::CONTINUOUS;

    return eExposureAutoLimitAuto::OFF;
}

nLucidVisionLabsConnect::nTriton::eExposureAuto lucid::to_exposure_auto(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "OFF"))        return eExposureAuto::OFF;
    if (nStringUtils::iequal(str, "ONCE"))       return eExposureAuto::ONCE;
    if (nStringUtils::iequal(str, "CONTINUOUS")) return eExposureAuto::CONTINUOUS;

    return eExposureAuto::OFF;
}

nLucidVisionLabsConnect::nTriton::eExposureTimeSelector lucid::to_exposure_time_selector(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "COMMON"))        return eExposureTimeSelector::COMMON;

    return eExposureTimeSelector::COMMON;
}

nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto lucid::to_balance_white_auto(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "OFF"))        return eBalanceWhiteAuto::OFF;
    if (nStringUtils::iequal(str, "ONCE"))       return eBalanceWhiteAuto::ONCE;
    if (nStringUtils::iequal(str, "CONTINUOUS")) return eBalanceWhiteAuto::CONTINUOUS;

    return eBalanceWhiteAuto::OFF;
}

nLucidVisionLabsConnect::nTriton::eGainAuto lucid::to_gain_auto(const std::string& str)
{
    using namespace nLucidVisionLabsConnect::nTriton;

    if (nStringUtils::iequal(str, "OFF"))        return eGainAuto::OFF;
    if (nStringUtils::iequal(str, "ONCE"))       return eGainAuto::ONCE;
    if (nStringUtils::iequal(str, "CONTINUOUS")) return eGainAuto::CONTINUOUS;

    return eGainAuto::OFF;
}
