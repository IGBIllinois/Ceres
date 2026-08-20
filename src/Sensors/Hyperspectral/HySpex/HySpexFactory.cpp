/**
 */

#include "HySpexFactory.hpp"
#include "HySpexIDs.hpp"
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "HySpexVNIR_3000N_Model_net.hpp"
#include "HySpexVNIR_3000N_Model_simulation.hpp"
#include "HySpexVNIR_3000N_Controller.hpp"
#include "HySpexVNIR_3000N_View.hpp"
#include "HySpexVNIR_3000N_StatusView.hpp"
#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"
#include "HySpexSWIR_384_Model_direct.hpp"
#include "HySpexSWIR_384_Model_net.hpp"
#include "HySpexSWIR_384_Controller.hpp"
#include "HySpexSWIR_384_View.hpp"
#include "HySpexSWIR_384_StatusView.hpp"
#include "HySpexSWIR_384_PropertyPage_Remote.hpp"

#include <hyspex_connect/HySpexCameraFactory.hpp>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QDebug>


namespace
{
    bool detect_cameras(const nlohmann::json& sensorInfo)
    {
        cHySpexCameraFactory mgr;

        if (mgr.numOfCameras() <= 0)
        {
            if (!sensorInfo.contains("settings"))
            {
                qCritical() << "The \"settings\" entry is missing!";
                return false;
            }

            std::string settings = sensorInfo["settings"];

            qInfo() << "Searching for HySpex Cameras...";

            if (!mgr.detectCameras(settings))
            {
                qCritical() << "No HySpex cameras were detected!";
                return false;
            }
        }

        return true;
    }
}


sSensorWidgets create_vnir_3000N_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the HySpex VNIR 3000N model and view...
    cHySpexVNIR_3000N_Model* pModel = nullptr; 

    if (!sensorInfo.contains("protocol"))
    {
        qCritical() << "The \"protocol\" entry is missing from the VNIR-3000N sensor section.  ";
        qCritical() << "Valid values are: direct, net, simulation.";
        return sSensorWidgets();
    }

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "direct")
    {
        if (!detect_cameras(sensorInfo))
            return sSensorWidgets();

        cHySpexCameraFactory mgr;
        auto camera = mgr.getVNIR_3000N();

        if (!camera)
        {
            qCritical() << "No HySpex VNIR-3000N cameras were detected!";
            return sSensorWidgets();
        }

        pModel = new cHySpexVNIR_3000N_Model_direct(std::move(camera));
    }
    else if (protocol == "net")
        pModel = new cHySpexVNIR_3000N_Model_net();
    else if (protocol == "simulation")
        pModel = new cHySpexVNIR_3000N_Model_simulation();

    if (!pModel)
        throw std::runtime_error("HySpex VNIR 3000N: Unknown protocol type!");


    if (no_visualization)
    {
        auto* pView = new cHySpexVNIR_3000N_StatusView(pModel);
        pView->createWidgets();
        pView->doLayout();

        pView->connectToModel();

        auto* pController = new cHySpexVNIR_3000N_Controller(pModel);

        pController->connectToModel();

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexVNIR_3000N_View(pModel, dockWidget);

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    
    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_swir_384_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the HySpex SWIR 384 model and view...
    cHySpexSWIR_384_Model* pModel = nullptr;

    if (!sensorInfo.contains("protocol"))
    {
        qCritical() << "The \"protocol\" entry is missing from the SWIR-384 sensor section.  ";
        qCritical() << "Valid values are: direct, net.";
        return sSensorWidgets();
    }

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "direct")
    {
        if (!detect_cameras(sensorInfo))
            return sSensorWidgets();

        cHySpexCameraFactory mgr;
        auto camera = mgr.getSWIR_384();

        if (!camera)
        {
            qCritical() << "No HySpex SWIR-384 cameras were detected!";
            return sSensorWidgets();
        }

        pModel = new cHySpexSWIR_384_Model_direct(std::move(camera));
    }
    else if (protocol == "net")
        pModel = new cHySpexSWIR_384_Model_net();

    if (!pModel)
        throw std::runtime_error("HySpex SWIR 384: Unknown protocol type!");


    if (no_visualization)
    {
        auto* pView = new cHySpexSWIR_384_StatusView(pModel);

        pView->createWidgets();
        pView->doLayout();

        pView->connectToModel();

        auto* pController = new cHySpexSWIR_384_Controller(pModel);

        pController->connectToModel();

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexSWIR_384_View(pModel, dockWidget);

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets hyspex::create_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    if (!sensorInfo.contains("sensor"))
    {
        qCritical() << "The \"sensor\" entry is missing from the \"hyspex\" sensor section.  ";
        qCritical() << "Valid values are: VNIR-3000N, SWIR-384.";
        return sSensorWidgets();
    }

    std::string sensor = sensorInfo["sensor"];

    if (sensor == vnir_3000N_id)
        return create_vnir_3000N_sensor(sensorInfo, no_visualization);

    if (sensor == swir_384_id)
        return create_swir_384_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void hyspex::remove_sensor(sSensorWidgets widgets)
{
    auto* pModel = widgets.pModel;

    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pView->disconnect();
    pModel->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}


cSensorPropertyPage* hyspex::create_sensor_property_page(const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (model == vnir_3000N_id)
    {
        auto page = new cHySpexVNIR_3000N_PropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    if (model == swir_384_id)
    {
        auto page = new cHySpexSWIR_384_PropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    return nullptr;
}
