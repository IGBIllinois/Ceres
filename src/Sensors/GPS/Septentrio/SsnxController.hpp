
#pragma once

#include "../GpsController.hpp"

class cSsnxModel;
class cGpsControllerNetEncoder;


class cSsnxController : public cGpsController
{
	Q_OBJECT

public:
    cSsnxController(cSsnxModel* pModel, QObject* parent = nullptr);

    void connectToModel() override;

private:
    cSsnxModel* mpModel = nullptr;
};


