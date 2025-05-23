
#pragma once

#include "../SensorController.hpp"
#include "GpsTypes.hpp"

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
    const std::string& instance() const override;
    bool has_instance() const override;

public slots:
    virtual void onReferenceComplete() = 0;

protected:
    void txReferenceData(cGpsControllerNetEncoder* encoder);
    void txReferenceParameters(cGpsControllerNetEncoder* encoder);
    void txReferenceState(cGpsControllerNetEncoder* encoder);
    void txReferenceState(cGpsControllerNetEncoder* encoder, gps::eReferenceState state);

protected:
    cGpsModel* mpModel = nullptr;
};


