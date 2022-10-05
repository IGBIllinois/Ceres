/*
 * The HySpexVNIR_3000N_PropertyPage to control the HySpex VNIR 3000N.
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


class cHySpexVNIR_3000N_PropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cHySpexVNIR_3000N_PropertyPage(QWidget* parent = nullptr);
    ~cHySpexVNIR_3000N_PropertyPage() = default;

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

    QString mModeDefault;
    double  mMinAzimuthAngleDefault_deg;
    double  mMaxAzimuthAngleDefault_deg;
};


