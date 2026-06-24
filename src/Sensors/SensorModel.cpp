
#include "SensorModel.hpp"


//Q_DECLARE_METATYPE(sensor::eStatus)


cSensorModel::cSensorModel(const std::string& name, QObject* parent)
:
    QObject(parent),
    mSensorName(name)
{
    qRegisterMetaType<sensor::eStatus>();

    mIsRecording = false;

    setObjectName(name.c_str());
}

cSensorModel::cSensorModel(const std::string& name, const std::string& instance, QObject* parent)
    : cSensorModel(name, parent)
{
    mSensorInstance = instance;

    QString title = QString::fromStdString(name);

    title += ":";
    title += QString::fromStdString(instance);

    setObjectName(title);
}

bool cSensorModel::configure(const nlohmann::json& jsonCfg)
{
    if (mManufacturer.empty())
    {
        if (!jsonCfg.contains("Manufacturer"))
            throw std::logic_error("Missing \"Manufacturer\" entry.");
        mManufacturer = jsonCfg["Manufacturer"];
    }

    if (mModel.empty())
    {
        if (!jsonCfg.contains("Model"))
            throw std::logic_error("Missing \"Model\" entry.");
        mModel = jsonCfg["Model"];
    }

    if (mSerialNumber.empty())
    {
        if (!jsonCfg.contains("Serial Number"))
            throw std::logic_error("Missing \"Serial Number\" entry.");
        mSerialNumber = jsonCfg["Serial Number"];
    }

    setStatus(sensor::eStatus::CONFIGURED);
    return true;
}

bool cSensorModel::isInitialized() const
{
    return mIsInitialized;
}

bool cSensorModel::initialize()
{
    mIsInitialized = true;
    setStatus(sensor::eStatus::INITIALIZED);
    return true;
};

void cSensorModel::setInstanceName(const std::string& instance)
{
    mSensorInstance = instance;
}

void cSensorModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mIsRecording = false;
}

void cSensorModel::disableDataRecording()
{
    mIsRecording = false;
}

void cSensorModel::dataRecordingStateChange(bool record)
{
    mIsRecording = record;
}


bool cSensorModel::isRecording()
{
    return mIsRecording;
}

void cSensorModel::setStatus(sensor::eStatus status)
{
    mStatus = status;
    emit sensorStatusChanging(q_name(), q_instance(), mStatus);
}

void cSensorModel::updateName(const std::string& name)
{
    QString old_name = QString::fromStdString(mSensorName);
    QString new_name = QString::fromStdString(name);
    QString instance = QString::fromStdString(mSensorInstance);

    mSensorName = name;

    emit sensorNameChanging(old_name, new_name, instance);
}

void cSensorModel::logMessage(quint8 type, QString msg)
{
    emit elogMessage(type, q_name(), q_instance(), msg);
}



