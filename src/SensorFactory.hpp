
#pragma once

#include <string>
#include <utility>
#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

class cSensorModel;

std::pair<cSensorModel*, QDockWidget*> create_sensor(const std::string& name, QWidget* parent);