/*
 * The OusterPropertyPage to control the OUSTER LiDAR.
 */

#pragma once

#include "../SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"

 // Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QComboBox;
class QGroupBox;
QT_END_NAMESPACE


class cOusterPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cOusterPropertyPage(QWidget* parent = nullptr);
    ~cOusterPropertyPage() = default;

    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type) override;

protected:
    QLabel*     mpModeLabel = nullptr;
    QComboBox*  mpModes = nullptr;

    QGroupBox*  mpAzimuthWindow = nullptr;
    QLabel*     mpMinAzimuthAngleLabel = nullptr;
    QLineEdit*  mpMinAzimuthAngle_deg = nullptr;
    QLabel*     mpMaxAzimuthAngleLabel = nullptr;
    QLineEdit*  mpMaxAzimuthAngle_deg = nullptr;
};


