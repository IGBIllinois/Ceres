
#include "DataVerifier.hpp"
#include "BlockDataFileExceptions.hpp"
#include "ParserExceptions.hpp"

#include <QDir>
#include <QFileInfo>


//-----------------------------------------------------------------------------
cDataVerifier::cDataVerifier(const QString& dataDir, QObject* parent)
    : QObject(parent)
{
    mCurrentDataDirectory = dataDir;
}

cDataVerifier::~cDataVerifier()
{}

//-----------------------------------------------------------------------------
bool cDataVerifier::open(const std::string& file_name)
{
    mCurrentFileName = QString::fromStdString(file_name);
    return mFileReader.open(file_name);
}

//-----------------------------------------------------------------------------
void cDataVerifier::run()
{
    if (!mFileReader.isOpen())
    {
        emit fileResults(false, "File is not open!");
        return;
    }

    try
    {
        while (!mFileReader.eof())
        {
            if (mFileReader.fail())
            {
                emit fileResults(false, "I/O Error: failbit is set.");
                mFileReader.close();
                return;
            }

//            try
            {
                mFileReader.processBlock();
            }
//            catch (const bdf::crc_error& e)
            {
                // CRC error are not necessarily corrupt files
//                emit statusMessage(e.what());
            }
        }
    }
    catch (const bdf::stream_error& e)
    {
        mFileReader.close();
        std::string msg = e.what();
        emit fileResults(false, e.what());

        moveFileToFailed();
        return;
    }
    catch (const std::exception& e)
    {
        if (mFileReader.eof())
        {
            emit fileResults(true, QString());
        }
        else
        {
            emit fileResults(false, e.what());
            moveFileToFailed();
        }
        mFileReader.close();
        return;
    }

    emit fileResults(true, QString());
    mFileReader.close();
}

//-----------------------------------------------------------------------------
void cDataVerifier::moveFileToFailed()
{
    if (mFileReader.isOpen())
        mFileReader.close();

    if (!QDir().exists(mCurrentDataDirectory))
        return;

    QString path = mCurrentDataDirectory;
    path += "/failed";

    QDir failedDir(path);
    if (!failedDir.exists())
    {
        if (!QDir().mkdir(path))
            return;
    }

    QString filename = QFileInfo(mCurrentFileName).fileName();
    QString newName = path + "/" + filename;

    QDir().rename(mCurrentFileName, newName);
}

//-----------------------------------------------------------------------------
void cDataVerifier::onConfigParam_2(const ouster::config_param_2_t& data)
{
}

void cDataVerifier::onSensorInfo_2(const ouster::sensor_info_2_t& data)
{

}

void cDataVerifier::onTimestamp_2(const ouster::timestamp_2_t& data)
{}

void cDataVerifier::onSyncPulseIn_2(const ouster::sync_pulse_in_2_t& data)
{}

void cDataVerifier::onSyncPulseOut_2(const ouster::sync_pulse_out_2_t& data)
{}

void cDataVerifier::onMultipurposeIo_2(const ouster::multipurpose_io_2_t& data)
{}

void cDataVerifier::onNmea_2(const ouster::nmea_2_t& data)
{}

void cDataVerifier::onTimeInfo_2(const ouster::time_info_2_t& data)
{}

void cDataVerifier::onBeamIntrinsics_2(const ouster::beam_intrinsics_2_t& data)
{
    if (data.altitude_angles_deg.empty() || data.azimuth_angles_deg.empty())
    {
        throw bdf::parse_error("Invalid beam intrinsics version 2!");
    }
}

void cDataVerifier::onImuIntrinsics_2(const ouster::imu_intrinsics_2_t& data)
{
    if (data.imu_to_sensor_transform.empty())
    {
        throw bdf::parse_error("Invalid imu intrinsics version 2!");
    }
}

void cDataVerifier::onLidarIntrinsics_2(const ouster::lidar_intrinsics_2_t& data)
{
    if (data.lidar_to_sensor_transform.empty())
    {
        throw bdf::parse_error("Invalid lidar intrinsics version 2!");
    }
}

void cDataVerifier::onLidarDataFormat_2(const ouster::lidar_data_format_2_t& data)
{}

void cDataVerifier::onImuData(const ouster::imu_data_t& data)
{}

void cDataVerifier::onLidarDataFormat_2(const ouster::lidar_data_format_2_3_t& data)
{}

void cDataVerifier::onLidarData(const ouster::lidar_data_frame_t& data)
{}

void cDataVerifier::onActiveCameraId(int id)
{
    if ((id < 1) || (id > 4))
    {
        throw bdf::parse_error("Invalid active camera id!");
    }
}

void cDataVerifier::onFramesPerSecond(int frames_per_sec)
{
    if ((frames_per_sec < 1) || (frames_per_sec > 30))
    {
        throw bdf::parse_error("Invalid frames per second!");
    }
}

void cDataVerifier::onBitmap(const QBitmap& in)
{}

void cDataVerifier::onJPEG(const QImage& image)
{}

void cDataVerifier::onMpegFrame(const QImage& image)
{}

void cDataVerifier::onImageSize(int width, int height)
{
    bool validWidth = (width == 480) || (width == 640) || (width == 800) ||
        (width == 854) || (width == 1024) || (width == 1280) || (width == 1920);

    bool validHeight = (height == 270) || (height == 300) || (height == 360)
        || (height == 400) || (height == 450) || (height == 480) || (height == 500)
        || (height == 600) || (height == 640) || (height == 720) || (height == 768)
        || (height == 1080);

    if (!validWidth || !validHeight)
    {
        throw bdf::parse_error("Invalid image size!");
    }
}


