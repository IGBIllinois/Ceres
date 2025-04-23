
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirIDs.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>
#include <QByteArray>
#include <QUrlQuery>

#include <nlohmann/json.hpp>

QByteArray to_QByteArray(const nlohmann::json& jsonDoc)
{
    std::string s = jsonDoc.dump();
    return QByteArray(s.c_str(), s.size());
}

cTeledyneFlirCameraModel::cTeledyneFlirCameraModel(const std::string& name, QObject* parent)
:
    cIrCameraModel(name, parent)
{
    mManufacturer = "Teledyne FLIR";

    mConnected = false;
}

cTeledyneFlirCameraModel::~cTeledyneFlirCameraModel()
{
	stopCommunications();
}

const char* cTeledyneFlirCameraModel::descriptor() const
{
    return teledyne_flir_id;
}

uint16_t cTeledyneFlirCameraModel::data_class_id() const
{
    return mSerializer.classID();
}

bool cTeledyneFlirCameraModel::configure(const nlohmann::json& jsonCfg)
{
    mConnected = false;

    return cIrCameraModel::configure(jsonCfg);
}

bool cTeledyneFlirCameraModel::startCommunications()
{
    if (!mConnected) return false;

	return true;
}

void cTeledyneFlirCameraModel::stopCommunications()
{
}

void cTeledyneFlirCameraModel::update()
{
}


