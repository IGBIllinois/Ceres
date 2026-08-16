
#include "AxisCommunicationsModel.hpp"
#include "AxisCommunicationsIDs.hpp"

#include "StringUtils.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>
#include <QByteArray>
#include <QUrlQuery>
#include <QImageWriter>

#include <nlohmann/json.hpp>

namespace
{
    QByteArray to_QByteArray(const nlohmann::json& jsonDoc)
    {
        std::string s = jsonDoc.dump();
        return QByteArray(s.c_str(), s.size());
    }
}

cAxisCommunicationsModel::cAxisCommunicationsModel(const std::string& name, QObject* parent)
:
    cRgbCameraModel(name, parent),
    mpHttpManager(nullptr),
    mVapixVersion(0)
{
    mManufacturer = "Axis Communications";

    mConnected = false;
    mpHttpManager = new QNetworkAccessManager(this);
}

cAxisCommunicationsModel::~cAxisCommunicationsModel()
{
	stopCommunications();
}

const char* cAxisCommunicationsModel::descriptor() const
{
    return axis_communications_id;
}

uint16_t cAxisCommunicationsModel::data_class_id() const
{
    return mSerializer.classID();
}

void cAxisCommunicationsModel::updateViews()
{
    emit modeChanged(static_cast<int>(mMode));
    emit lapseIntervalChanged(mLapseInterval_ms);
}

bool cAxisCommunicationsModel::autoEmitImages() const { return mAutoEmitImages; }
void cAxisCommunicationsModel::autoEmitImages(bool auto_emit_images) { mAutoEmitImages = auto_emit_images; }

const QImage& cAxisCommunicationsModel::getCurrentImage() const
{
    return mCurrentImage;
}

int cAxisCommunicationsModel::getVapixVersion() const
{
    return mVapixVersion;
}

const std::vector<rgb::sImageSize_t>& cAxisCommunicationsModel::getImageSizes() const
{
    return mSupportedImageSizes;
}

const std::vector<rgb::eIMAGE_FORMAT>& cAxisCommunicationsModel::getImageFormats() const
{
    return mSupportedImageFormats;
}

bool cAxisCommunicationsModel::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        std::string url = jsonCfg["url"];
        mUrl = QUrl(QString(url.c_str()));

        std::string mode = "video";

        if (jsonCfg.contains("mode"))
            mode = jsonCfg["mode"];

        int32_t lapse_interval_ms = -1;

        if (jsonCfg.contains("time-lapse interval (s)"))
            lapse_interval_ms = static_cast<int32_t>(jsonCfg["time-lapse interval (s)"].get<float>() * 1000.0);
        else if (jsonCfg.contains("time-lapse interval (ms)"))
            lapse_interval_ms = jsonCfg["time-lapse interval (ms)"].get<int32_t>();
        else if (jsonCfg.contains("lapse interval (s)"))
            lapse_interval_ms = static_cast<int32_t>(jsonCfg["lapse interval (s)"].get<float>() * 1000.0);
        else if (jsonCfg.contains("lapse interval (ms)"))
            lapse_interval_ms = jsonCfg["lapse interval (ms)"].get<int32_t>();
        else if (jsonCfg.contains("interval (s)"))
            lapse_interval_ms = static_cast<int32_t>(jsonCfg["interval (s)"].get<float>() * 1000.0);
        else if (jsonCfg.contains("interval (ms)"))
            lapse_interval_ms = jsonCfg["interval (ms)"].get<int32_t>();

        if (nStringUtils::iequal(mode, "photo"))
        {
            setMode(eMode::SINGLE);
        }
        else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
        {
            setMode(eMode::TIME_LAPSE);

            if (lapse_interval_ms < 0)
            {
                throw std::logic_error("Missing \"time-lapse interval (ms)\" entry!");
            }
        }
        else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
        {
            setMode(eMode::CONTINUOUS);
        }
        else
            throw std::logic_error("Unknown \"mode\" entry!  Values can be \"photo\", \"time lapse\", or \"video\".");

        if (lapse_interval_ms > 0)
            setLapseInterval_ms(lapse_interval_ms);
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"axis_communications\" configuration: ";
        str.append(e.what());
        logMessage(logERROR, str);
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    emit statusMessage("Quering Axis Communications server...");

    if (!queryVapixSupport()) return false;
    if (!querySupportedResolutions()) return false;
    if (!querySupportedImageFormats()) return false;

    mConnected = true;

    return cRgbCameraModel::configure(jsonCfg);
}

bool cAxisCommunicationsModel::startCommunications()
{
    if (!mConnected) return false;

	if (mpHttpManager) return true;

	mpHttpManager  = new QNetworkAccessManager(this);

	return true;
}

void cAxisCommunicationsModel::stopCommunications()
{
    if (!mpHttpManager) return;

    delete mpHttpManager; mpHttpManager = nullptr;
}

void cAxisCommunicationsModel::requestMode(int mode)
{
    if ((mode >= eMode::SINGLE) && (mode <= eMode::CONTINUOUS))
        setMode(static_cast<eMode>(mode));
}

void cAxisCommunicationsModel::requestImageSize(int width, int height)
{
    updateImageSize(width, height);
}

void cAxisCommunicationsModel::requestFrameRate_Hz(double frame_rate_hz)
{
    setFrameRate_Hz(frame_rate_hz);
}

void cAxisCommunicationsModel::requestLapseInterval_ms(uint32_t interval_ms)
{
    setLapseInterval_ms(interval_ms);
}

void cAxisCommunicationsModel::onDefaultDataPathChange(QString path)
{}

void cAxisCommunicationsModel::requestImage()
{
    mImageRequested = true;
}

void cAxisCommunicationsModel::requestImages(bool auto_emit)
{
    mAutoEmitImages = auto_emit;
}

void cAxisCommunicationsModel::requestSaveImage()
{
    QString fileName = "Image_";
    fileName += QString::number(1);
    fileName += ".jpg";
    QImageWriter writer(fileName);
    writer.setFormat("JPEG");
    writer.write(mCurrentImage);
}

void cAxisCommunicationsModel::takePhoto(bool update_view)
{
    if (mMode == eMode::SINGLE)
    {
        mPhotoRequested = true;
        mImageRequested = update_view;
        mAutoEmitImages = false;
        mSavePhoto = false;
    }
}

void cAxisCommunicationsModel::takePhoto(bool update_view, bool auto_save)
{
    if (mMode == eMode::SINGLE)
    {
        mPhotoRequested = true;
        mImageRequested = update_view;
        mAutoEmitImages = false;
        mSavePhoto = auto_save;
    }
}


void cAxisCommunicationsModel::requestReceived(QNetworkReply* pReply)
{
	pReply->deleteLater();

    if (pReply->error() == QNetworkReply::NoError) 
    {
        // Get the http status code
        int v = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            // Here we got the final reply 
            QString replyText = pReply->readAll();
            processReply(replyText.toStdString());
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative, 
            // we have to use the previous one to resolve it 
            mUrl = pReply->url().resolved(newUrl);
        }
    }
    else
    {
        // Error
        pReply->errorString();
    }
}

void cAxisCommunicationsModel::update()
{
}

bool cAxisCommunicationsModel::queryVapixSupport()
{
    QUrl url(mUrl);

    url.setPath("/axis-cgi/param.cgi");

    QUrlQuery query;
    query.addQueryItem("action", "list");
    query.addQueryItem("group", "Properties.API.HTTP.Version");

    url.setQuery(query);

    QNetworkRequest request(url);

    QString replyText = queryServer(request);

    if (!replyText.isEmpty())
    {
        // Here we got the final reply 
        query.setQuery(replyText);
        auto version = query.queryItemValue("Properties.API.HTTP.Version", QUrl::FullyDecoded);
        mVapixVersion = version.toInt();
        return true;
    }

    return false;
}

bool cAxisCommunicationsModel::querySupportedResolutions()
{
    mSupportedImageSizes.clear();

    QUrl url(mUrl);

    url.setPath("/axis-cgi/param.cgi");

    QUrlQuery query;
    query.addQueryItem("action", "list");
    query.addQueryItem("group", "Properties.Image.Resolution");

    url.setQuery(query);

    auto s = url.toString();
    QNetworkRequest request(url);

    QString replyText = queryServer(request);

    if (!replyText.isEmpty())
    {
        // Here we got the final reply 
        query.setQuery(replyText);
        QString resolution = query.queryItemValue("Properties.Image.Resolution", QUrl::FullyDecoded);
        auto image_sizes = resolution.split(',');
        for (auto image_size : image_sizes)
        {
            image_size.remove(QChar('\n'));
            mSupportedImageSizes.push_back(axis::to_image_size(image_size.toStdString()));
        }

        return true;
    }

    return false;
}

bool cAxisCommunicationsModel::querySupportedImageFormats()
{
    mSupportedImageFormats.clear();

    QUrl url(mUrl);

    url.setPath("/axis-cgi/param.cgi");

    QUrlQuery query;
    query.addQueryItem("action", "list");
    query.addQueryItem("group", "Properties.Image.Format");

    url.setQuery(query);

    QNetworkRequest request(url);

    QString replyText = queryServer(request);

    if (!replyText.isEmpty())
    {
        // Here we got the final reply 
        query.setQuery(replyText);
        auto format_list = query.queryItemValue("Properties.Image.Format", QUrl::FullyDecoded);
        auto formats = format_list.split(',');
        for (auto format : formats)
        {
            format.remove(QChar('\n'));
            mSupportedImageFormats.push_back(axis::to_image_format(format.toStdString()));
        }

        return true;
    }

    return false;
}

rgb::sImageSize_t cAxisCommunicationsModel::queryImageResolution(uint8_t camera)
{
    QUrl url(mUrl);

    url.setPath("/axis-cgi/imagesize.cgi");

    QUrlQuery query;
    query.addQueryItem("camera", QString::number(camera));

    url.setQuery(query);

    QNetworkRequest request(url);

    QString replyText = queryServer(request);

    if (!replyText.isEmpty())
    {
        rgb::sImageSize_t image_size;

        auto dimensions = replyText.split('\n');
        int n = dimensions[0].lastIndexOf('=') + 1;
        auto width = dimensions[0].right(dimensions[0].size() - n);
        image_size.width = width.toInt();

        n = dimensions[1].lastIndexOf('=') + 1;
        auto height = dimensions[1].right(dimensions[1].size() - n);
        image_size.height = height.toInt();

        return image_size;
    }

    return rgb::sImageSize_t();
}

QString cAxisCommunicationsModel::queryServer(const QNetworkRequest& request)
{
    QNetworkReply* reply = mpHttpManager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    reply->deleteLater();

    QString replyText;

    if (reply->error() == QNetworkReply::NoError)
    {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            // Here we got the final reply 
            replyText = reply->readAll();
        }
    }
    else
    {
        // Error
        QString msg = "Axis Communication Error: ";
        msg += reply->errorString();
        logMessage(logERROR, msg);
    }

    return replyText;
}

QBitmap cAxisCommunicationsModel::getBitmap(int cameraId, rgb::sImageSize_t resolution)
{
    if (std::find(mSupportedImageFormats.begin(), mSupportedImageFormats.end(), rgb::eIMAGE_FORMAT::BITMAP)
        == mSupportedImageFormats.end())
    {
        return QBitmap();
    }

    if (resolution != rgb::sImageSize_t())
    {
        auto it = std::find(mSupportedImageSizes.begin(), mSupportedImageSizes.end(), resolution);
        if (it == mSupportedImageSizes.end())
            return QBitmap();
    }

    QUrl url(mUrl);

    url.setPath("/axis-cgi/bitmap/image.bmp");

    QUrlQuery query;

    if (cameraId > 0)
        query.addQueryItem("camera", QString::number(cameraId));

    if ((resolution.width != 0) && (resolution.height != 0))
        query.addQueryItem("resolution", QString(axis::to_string(resolution).c_str()));

    if (!query.isEmpty())
    {
        url.setQuery(query);
    }

    QNetworkRequest request(url);

    QNetworkReply* reply = mpHttpManager->get(request);

    reply->waitForReadyRead(5000);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    reply->deleteLater();

    QByteArray image_data = reply->readAll();
    QImage image;
    auto ok = image.loadFromData(image_data);

    return QBitmap::fromImage(image);
}

QImage cAxisCommunicationsModel::getJPEG(int cameraId, rgb::sImageSize_t resolution)
{
    if (std::find(mSupportedImageFormats.begin(), mSupportedImageFormats.end(), rgb::eIMAGE_FORMAT::JPEG)
        == mSupportedImageFormats.end())
    {
        return QImage();
    }

    if (resolution != rgb::sImageSize_t())
    {
        auto it = std::find(mSupportedImageSizes.begin(), mSupportedImageSizes.end(), resolution);
        if (it == mSupportedImageSizes.end())
            return QImage();
    }

    QUrl url(mUrl);

    url.setPath("/axis-cgi/jpg/image.cgi");

    QUrlQuery query;

    if (cameraId > 0)
        query.addQueryItem("camera", QString::number(cameraId));

    if ((resolution.width != 0) && (resolution.height != 0))
        query.addQueryItem("resolution", QString(axis::to_string(resolution).c_str()));

    if (!query.isEmpty())
    {
        url.setQuery(query);
    }

    QNetworkRequest request(url);

    QNetworkReply* reply = mpHttpManager->get(request);

    reply->waitForReadyRead(5000);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    reply->deleteLater();

    QByteArray image_data = reply->readAll();
    QImage image;
    auto ok = image.loadFromData(image_data);

    return image;
}

