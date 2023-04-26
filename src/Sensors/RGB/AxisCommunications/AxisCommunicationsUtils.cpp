
#include "AxisCommunicationsUtils.hpp"

#include <QImageWriter>
#include <QImageReader>
#include <QIODevice>

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
        cImageReadAdapter(const cImageBuffer* byteArray)
            : mpData(byteArray)
        {
            QIODevice::open(QIODevice::ReadOnly | QIODevice::Unbuffered);
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
        const cImageBuffer* mpData;
    };
}

std::string axis::to_string(rgb::eIMAGE_FORMAT format)
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


rgb::eIMAGE_FORMAT axis::to_image_format(const std::string& str)
{
    if (str == "mjpeg")
        return rgb::eIMAGE_FORMAT::MJPEG;
    if (str == "h264")
        return rgb::eIMAGE_FORMAT::H264;
    if (str == "bitmap")
        return rgb::eIMAGE_FORMAT::BITMAP;

    return rgb::eIMAGE_FORMAT::JPEG;
}


std::string axis::to_string(rgb::sImageSize_t size)
{
    std::string result = std::to_string(size.width);
    result += "x";
    result += std::to_string(size.height);

    return result;
}

rgb::sImageSize_t axis::to_image_size(const std::string& str)
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

void axis::to_buffer(const QBitmap& img, cBitmapBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "bmp");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img.toImage());
}

void axis::to_buffer(const QImage& img, cJpegBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "jpeg");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img);
}

void axis::to_buffer(const QImage& img, cMpegFrameBuffer& out)
{
    cImageWriteAdapter imageBuffer(&out);

    QImageWriter  imageWriter(&imageBuffer, "jpeg");

    out.clear();
    imageBuffer.seek(0);

    imageWriter.write(img);
}

void axis::to_image(const cBitmapBuffer& in, QBitmap& out)
{
    cImageReadAdapter imageBuffer(&in);

    QImageReader  imageReader(&imageBuffer, "bmp");

    out.clear();
    imageBuffer.seek(0);

    QImage image;
    imageReader.read(&image);
    out.fromImage(image);
}

void axis::to_image(const cJpegBuffer& in, QImage& out)
{
    cImageReadAdapter imageBuffer(&in);

    QImageReader  imageReader(&imageBuffer, "jpeg");

    imageBuffer.seek(0);

    imageReader.read(&out);
}

void axis::to_image(const cMpegFrameBuffer& img, QImage& out)
{
    cImageReadAdapter imageBuffer(&img);

    QImageReader  imageReader(&imageBuffer, "jpeg");

    imageBuffer.seek(0);

    imageReader.read(&out);
}
