/**
 */

#include "HySpexFactory.hpp"
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "HySpexVNIR_3000N_Model_net.hpp"
#include "HySpexVNIR_3000N_View.hpp"
#include "HySpexSWIR_384_Model_direct.hpp"
#include "HySpexSWIR_384_Model_net.hpp"
#include "HySpexSWIR_384_View.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

sSensorWidgets create_vnir_3000N_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the HySpex VNIR 3000N model and view...
    cHySpexVNIR_3000N_Model* pModel = nullptr; 

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "direct")
        pModel = new cHySpexVNIR_3000N_Model_direct();
    else if (protocol == "net")
        pModel = new cHySpexVNIR_3000N_Model_net();

    if (!pModel)
        throw std::runtime_error("HySpex VNIR 3000N: Unknown protocol type!");


    if (no_visualization)
    {
        return sSensorWidgets(pModel);
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
        pModel = new cHySpexSWIR_384_Model_direct();
    else if (protocol == "net")
        pModel = new cHySpexSWIR_384_Model_net();

    if (!pModel)
        throw std::runtime_error("HySpex SWIR 384: Unknown protocol type!");


    if (no_visualization)
    {
        return sSensorWidgets(pModel);
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
    return nullptr;
}
