/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams.
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


class cAxisCommunicationsPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cAxisCommunicationsPropertyPage(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage() = default;

    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type) override;

protected:
    QLabel* mpCameraIdLabel = nullptr;
    QLineEdit* mpCameraId = nullptr;

    QLabel* mpImageSizeLabel = nullptr;
    QComboBox* mpImageSizes = nullptr;

    QLabel* mpFrameRateLabel = nullptr;
    QLineEdit* mpFrameRate_fps = nullptr;

    int mDefaultCameraId = -1;
    QString mDefaultImageSize;
    int  mDefaultFrameRate_fps = -1;
};


