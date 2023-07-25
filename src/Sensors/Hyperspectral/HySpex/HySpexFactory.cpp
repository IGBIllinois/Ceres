/**
 */

#include "HySpexFactory.hpp"
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

#include <HySpexConnect/HySpexCameraFactory.hpp>

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

        QObject::connect(pModel, &cSensorModel::sensorStatusChanging, pView, &cSensorStatusView::onSensorStatusChange);

        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::initStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onInitStatusChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::commStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onCommStatusChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::acqStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onAcqStatusChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::bgStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onBgStatusChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::coolingStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onCoolingStatusChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::shutterStatusChanged, pView, &cHySpexVNIR_3000N_StatusView::onShutterStatusChange);

        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::lensInfoChanged, pView, &cHySpexVNIR_3000N_StatusView::onLensInfoChange);

        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::avgFramesChanged, pView, &cHySpexVNIR_3000N_StatusView::onAvgFramesChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::framePeriodChanged, pView, &cHySpexVNIR_3000N_StatusView::onFramePeriodChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::minFramePeriodChanged, pView, &cHySpexVNIR_3000N_StatusView::onMinFramePeriodChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::integrationTimeChanged, pView, &cHySpexVNIR_3000N_StatusView::onIntegrationTimeChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::maxIntegrationTimeChanged, pView, &cHySpexVNIR_3000N_StatusView::onMaxIntegrationTimeChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::ambientTempChanged, pView, &cHySpexVNIR_3000N_StatusView::onAmbientTempChange);
        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::sensorTempChanged, pView, &cHySpexVNIR_3000N_StatusView::onSensorTempChange);


        auto* pController = new cHySpexVNIR_3000N_Controller(pModel);

        QObject::connect(pModel, &cHySpexVNIR_3000N_Model::backgroundComplete, pController, &cHySpexVNIR_3000N_Controller::onBackgroundComplete);

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexVNIR_3000N_View(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    
    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_swir_384_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the HySpex SWIR 384 model and view...
    cHySpexSWIR_384_Model* pModel = nullptr;

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

        QObject::connect(pModel, &cSensorModel::sensorStatusChanging, pView, &cSensorStatusView::onSensorStatusChange);

        QObject::connect(pModel, &cHySpexSWIR_384_Model::initStatusChanged, pView, &cHySpexSWIR_384_StatusView::onInitStatusChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::commStatusChanged, pView, &cHySpexSWIR_384_StatusView::onCommStatusChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::acqStatusChanged, pView, &cHySpexSWIR_384_StatusView::onAcqStatusChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::bgStatusChanged, pView, &cHySpexSWIR_384_StatusView::onBgStatusChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::coolingStatusChanged, pView, &cHySpexSWIR_384_StatusView::onCoolingStatusChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::shutterStatusChanged, pView, &cHySpexSWIR_384_StatusView::onShutterStatusChange);

        QObject::connect(pModel, &cHySpexSWIR_384_Model::lensInfoChanged, pView, &cHySpexSWIR_384_StatusView::onLensInfoChange);

        QObject::connect(pModel, &cHySpexSWIR_384_Model::avgFramesChanged, pView, &cHySpexSWIR_384_StatusView::onAvgFramesChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::framePeriodChanged, pView, &cHySpexSWIR_384_StatusView::onFramePeriodChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::minFramePeriodChanged, pView, &cHySpexSWIR_384_StatusView::onMinFramePeriodChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::integrationTimeChanged, pView, &cHySpexSWIR_384_StatusView::onIntegrationTimeChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::maxIntegrationTimeChanged, pView, &cHySpexSWIR_384_StatusView::onMaxIntegrationTimeChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::ambientTempChanged, pView, &cHySpexSWIR_384_StatusView::onAmbientTempChange);
        QObject::connect(pModel, &cHySpexSWIR_384_Model::sensorTempChanged, pView, &cHySpexSWIR_384_StatusView::onSensorTempChange);


        auto* pController = new cHySpexSWIR_384_Controller(pModel);

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexSWIR_384_View(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets hyspex::create_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "VNIR-3000N")
        return create_vnir_3000N_sensor(sensorInfo, no_visualization);

    if (sensor == "SWIR-384")
        return create_swir_384_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

bool hyspex::remove_sensor(sSensorWidgets widgets)
{
    // Hyspex model and view...
    return false;
}


cSensorPropertyPage* hyspex::create_sensor_property_page(const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (model == "VNIR-3000N")
    {
        auto page = new cHySpexVNIR_3000N_PropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    if (model == "SWIR-384")
    {
        auto page = new cHySpexSWIR_384_PropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    return nullptr;
}
