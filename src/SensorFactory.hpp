
#pragma once

#include <string>
#include <utility>
#include <QWidget>
#include <QDockWidget>

class cSensorModel;

std::pair<cSensorModel*, QDockWidget*> create_sensor(const std::string& name, QWidget* parent);