
#include "SpidercamModel.hpp"

#include <QMessageBox>


cSpidercamModel::cSpidercamModel()
{

}

cSpidercamModel::~cSpidercamModel()
{

}

void cSpidercamModel::configure(const nlohmann::json& jsonCfg)
{
    std::string c2_ip;
    std::string c3_ip;
    uint16_t port = 0;

    double minX_mm = 0;
    double maxX_mm = 0;
    double minY_mm = 0;
    double maxY_mm = 0;
    double minHeight_mm = 0;
    double maxHeight_mm = 0;
    double maxSpeed_mmps = 0;

    try
    {
        c2_ip = static_cast<std::string>(jsonCfg["c2_ip"]);
        c3_ip = static_cast<std::string>(jsonCfg["c3_ip"]);
        port = jsonCfg["port"];

        minX_mm = jsonCfg["min X position (m)"] * M_TO_MM;
        maxX_mm = jsonCfg["max X position (m)"] * M_TO_MM;
        minY_mm = jsonCfg["min Y position (m)"] * M_TO_MM;
        maxY_mm = jsonCfg["max Y position (m)"] * M_TO_MM;
        minHeight_mm = jsonCfg["min height (m)"] * M_TO_MM;
        maxHeight_mm = jsonCfg["max height (m)"] * M_TO_MM;
        maxSpeed_mmps = jsonCfg["max speed (m/s)"] * M_TO_MM;
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ssnx\" configuration: ";
        str.append(e.what());
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }

    spidercam::sWorkingDimensions limits = { static_cast<uint32_t>(minX_mm), static_cast<uint32_t>(maxX_mm), 
        static_cast<uint32_t>(minY_mm), static_cast<uint32_t>(maxY_mm),
        static_cast<uint32_t>(minHeight_mm), static_cast<uint32_t>(maxHeight_mm) };

    mController.setSafetyLimits(limits, maxSpeed_mmps);

    QString msg("Trying to establishing connection to Spidercam at ");
    msg.append(c2_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

}

void cSpidercamModel::writeDataHeader(cBlockDataFile& file)
{

}

