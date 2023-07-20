/*
 * The HySpexCamera_PropertyPage to control the HySpex VNIR/SWIR Cameras.
 */

#pragma once

#include "../../SensorPropertyPage.hpp"
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


class cHySpexCamera_PropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cHySpexCamera_PropertyPage(QWidget* parent = nullptr);
    ~cHySpexCamera_PropertyPage() = default;

    void createWidgets() override;

public:
    cExperimentState* createState(const std::string& type) override;

protected:
    QGroupBox* getLensLayout();
    QGroupBox* getAcquisitionLayout();
    QGroupBox* getBackgroundLayout();

protected:
    QLabel*     mpLensLabel = nullptr;
    QComboBox*  mpLenses = nullptr;

    QLabel* mpAvgFramesLabel = nullptr;
    QLineEdit* mpAvgFrames = nullptr;

    QLabel* mpFramePeriodLabel = nullptr;
    QLineEdit* mpFramePeriod_us = nullptr;

    QLabel* mpMinFramePeriodLabel = nullptr;
    QLineEdit* mpMinFramePeriod_us = nullptr;

    QLabel* mpIntegrationTimeLabel = nullptr;
    QLineEdit* mpIntegrationTime_us = nullptr;

    QLabel* mpMaxIntegrationTimeLabel = nullptr;
    QLineEdit* mpMaxIntegrationTime_us = nullptr;


    QLabel* mpNumBackgroundsLabel = nullptr;
    QLineEdit* mpNumBackgrounds = nullptr;

    QPushButton* mpDoBackground = nullptr;

    QString mLensDefault;
};


