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

sSensorWidgets create_vnir_3000N_sensor()
{
    // Create the HySpex VNIR 3000N model and view...
    auto* pModel = new cHySpexVNIR_3000N_Model();
    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexVNIR_3000N_View(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    
    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_swir_384_sensor()
{
    // Create the HySpex SWIR 384 model and view...
    auto* pModel = new cHySpexSWIR_384_Model();
    auto* dockWidget = new QDockWidget();
    auto* pView = new cHySpexSWIR_384_View(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_hyspex_sensor(const nlohmann::json& sensorInfo)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "VNIR-3000N")
        return create_vnir_3000N_sensor();

    if (sensor == "SWIR-384")
        return create_swir_384_sensor();

    return sSensorWidgets();
}

void remove_hyspex_sensor(sSensorWidgets widgets)
{
    // Hyspex model and view...
}
