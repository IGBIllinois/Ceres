
#pragma once

#include <string>
#include <utility>

class cSensorModel;
class QWidget;
class QDockWidget;

static char* ouster_id = "ouster";

std::pair<cSensorModel*, QDockWidget*> create_ouster_sensor(QWidget* parent);