
#pragma once

#include "../SensorController.hpp"

class cGpsModel;
class cGpsControllerNetEncoder;


class cGpsController : public cSensorController
{
	Q_OBJECT

public:
    cGpsController(cGpsModel* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;

public slots:
    virtual void onBackgroundComplete() = 0;

protected:
    void txCurrentState(cGpsControllerNetEncoder* encoder);
    void txLensNames(cGpsControllerNetEncoder* encoder);
    void txShutterState(cGpsControllerNetEncoder* encoder);

private:
    cGpsModel* mpModel = nullptr;
};


