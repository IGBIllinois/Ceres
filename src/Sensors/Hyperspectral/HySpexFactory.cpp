/**
 */

#include "HySpexFactory.hpp"
#include "HySpexVNIR_3000N_Model.hpp"
#include "HySpexVNIR_3000N_View.hpp"
#include "HySpexSWIR_384_Model.hpp"
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
        pModel = new cHySpexVNIR_3000N_Model();
    else if (protocol == "net")
        pModel = new cHySpexVNIR_3000N_Model();
    else if (protocol == "file")
        pModel = new cHySpexVNIR_3000N_Model();

    if (!pModel)
        throw std::runtime_error("HySpex VNIR 3000N: Unknown protocol type!");


    if (no_visualization)
        return sSensorWidgets(pModel, nullptr);

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
        pModel = new cHySpexSWIR_384_Model();
    else if (protocol == "net")
        pModel = new cHySpexSWIR_384_Model();
    else if (protocol == "file")
        pModel = new cHySpexSWIR_384_Model();

    if (!pModel)
        throw std::runtime_error("HySpex SWIR 384: Unknown protocol type!");


    if (no_visualization)
        return sSensorWidgets(pModel, nullptr);

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexSWIR_384_View(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_hyspex_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "VNIR-3000N")
        return create_vnir_3000N_sensor(sensorInfo, no_visualization);

    if (sensor == "SWIR-384")
        return create_swir_384_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void remove_hyspex_sensor(sSensorWidgets widgets)
{
    // Hyspex model and view...
}
