/*
 * The GpsPropertyPage to control the GPS.
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


class cGpsPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cGpsPropertyPage(QWidget* parent = nullptr);
    ~cGpsPropertyPage() = default;

    void createWidgets() override;
    void doLayout() override;

    virtual void doCalcReference() = 0;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    QGroupBox* getReferenceLayout();

protected:
    QLabel* mpMinIntegrationTimeLabel = nullptr;
    QLineEdit* mpMinIntegrationTime_sec = nullptr;

    QLabel* mpMaxIntegrationTimeLabel = nullptr;
    QLineEdit* mpMaxIntegrationTime_sec = nullptr;

    QLabel* mpErrorThresholdLabel = nullptr;
    QLineEdit* mpErrorThreshold_mm = nullptr;

    QPushButton* mpDoReference = nullptr;

    QLineEdit* mpRef_X_mm = nullptr;
    QLineEdit* mpRef_Y_mm = nullptr;
    QLineEdit* mpRef_Z_mm = nullptr;
    QLineEdit* mpRef_Error_mm = nullptr;

protected:
    std::uint16_t mDefaultMinIntegrationTime_sec = 0;
    std::uint16_t mDefaultMaxIntegrationTime_sec = 0;
    std::uint16_t mDefaultErrorThreshold_mm = 0;
};


