
#pragma once

#include "../../SensorController.hpp"

class cHySpexCameraModel;
class cHySpexCamera_ControllerNetEncoder;


class cHySpexCamera_Controller : public cSensorController
{
	Q_OBJECT

public:
    cHySpexCamera_Controller(cHySpexCameraModel* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;

protected:
    void txCurrentState(cHySpexCamera_ControllerNetEncoder* encoder);
    void txLensNames(cHySpexCamera_ControllerNetEncoder* encoder);

private:
    cHySpexCameraModel* mpModel = nullptr;
};


