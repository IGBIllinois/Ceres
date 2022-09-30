/*
 * The OusterPropertyPage to control the OUSTER LiDAR.
 */

#pragma once

#include "../SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"

 // Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QListWidget;
class QTextEdit;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE


class cOusterPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cOusterPropertyPage(QWidget* parent = nullptr);
    ~cOusterPropertyPage() = default;

public:
    cExperimentState* createState(const std::string& type) override;

protected:
    QComboBox* mpModes = nullptr;
    QTextEdit* mpMinAzimuthAngle_deg = nullptr;
    QTextEdit* mpMaxAzimuthAngle_deg = nullptr;
};


