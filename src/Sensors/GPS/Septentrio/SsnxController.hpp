
#pragma once

#include "../GpsController.hpp"

class cSsnxModel;
class cGpsControllerNetEncoder;


class cSsnxController : public cGpsController
{
	Q_OBJECT

public:
    cSsnxController(QObject* parent = nullptr);
};


