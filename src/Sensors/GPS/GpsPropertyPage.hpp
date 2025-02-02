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

    virtual void doCalcReference() = 0;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    QGroupBox* getReferenceLayout();

protected:
    QLabel* mpIntegrationTimeLabel = nullptr;
    QLineEdit* mpIntegrationTime_sec = nullptr;

    QLabel* mpMaxIntegrationTimeLabel = nullptr;
    QLineEdit* mpMaxIntegrationTime_sec = nullptr;

    QPushButton* mpDoReference = nullptr;

protected:
    std::uint16_t mDefaultIntegrationTime_sec = 0;
    std::uint16_t mDefaultMaxIntegrationTime_sec = 0;
};


