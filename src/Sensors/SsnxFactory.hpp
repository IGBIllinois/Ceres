
#pragma once

#include <string>
#include <utility>

class cSensorModel;
class QWidget;
class QDockWidget;

static char* ssnx_id = "ssnx";

std::pair<cSensorModel*, QDockWidget*> create_ssnx_sensor(QWidget* parent);