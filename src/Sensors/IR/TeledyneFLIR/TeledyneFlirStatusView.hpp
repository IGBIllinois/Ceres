
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "ThermalImageWidget.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

// Forward Declaration
class cTeledyneFlirCameraModel;

class cTeledyneFlirStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cTeledyneFlirStatusView(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cTeledyneFlirStatusView();

	void createWidgets() override;
	void doLayout() override;

	void connectToModel() override;

signals:
	void requestMode(int mode);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestLapseInterval_ms(uint32_t interval_ms);

	void requestImage();
	void requestImages(bool update_view);

public slots:
	void onModeChange(int mode);
	void onLapseIntervalChange(int interval_ms);
	void onFrameRateChange(double rate_fps);
	void onImageSizeChange(int width, int height);
	void imageUpdated(const QImage& image);

	void resizeEvent(QResizeEvent*) override;

protected slots:
	void modeTextChanged(const QString&);
	void frameRateEditingFinished();
	void lapseIntervalEditingFinished();

private:
	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QPushButton* mpGrabImage = nullptr;

	cThermalImageWidget* mpThermalImage = nullptr;

private:
	cTeledyneFlirCameraModel* mpModel = nullptr;
};