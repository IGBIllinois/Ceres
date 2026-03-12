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
class QPushButton;
QT_END_NAMESPACE


class cTeledyneFlirPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
    Q_OBJECT

public:
    cTeledyneFlirPropertyPage(QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage() = default;

    void createWidgets() override;

    void enableControls(bool enable) override;
    
    void doLayout() override;

protected slots:
    virtual void onGrabImagePressed() = 0;

protected:
    QLabel* mpModeLabel = nullptr;
    QComboBox* mpMode = nullptr;

    QLabel* mpImageSizeLabel = nullptr;
    QLineEdit* mpImageSize = nullptr;

    QLabel* mpFrameRateLabel = nullptr;
    QLineEdit* mpFrameRate_fps = nullptr;

    QLabel* mpFrameIntervalLabel = nullptr;
    QLineEdit* mpFrameInterval_s = nullptr;

    QLabel* mpThermalRangeLabel = nullptr;
    QLineEdit* mpThermalRange = nullptr;

    QPushButton* mpGrabImage = nullptr;
};


