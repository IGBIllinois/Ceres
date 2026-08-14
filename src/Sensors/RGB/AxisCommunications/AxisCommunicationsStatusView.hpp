
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "RgbImageWidget.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

// Forward Declaration
class cAxisCommunicationsModel;

class cAxisCommunicationsStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cAxisCommunicationsStatusView(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsStatusView();

	void createWidgets() override;
	void doLayout() override;

signals:
	void requestImage();
	void saveImage();

public slots:
	void onCameraIdChange(int id);
	void onImageSizeChange(int width, int height);
	void onModeChange(int mode);
	void onFrameRateChange(int rate_fps);
	void onLapseIntervalChange(int interval_ms);

	void imageUpdated(const QImage& image);

	void resizeEvent(QResizeEvent*) override;

private:
	QLabel* mpCameraIdLabel = nullptr;
	QLineEdit* mpCameraId = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

	QPushButton* mpGrabImage = nullptr;

//	QPushButton* mpSaveImage = nullptr;

	cRgbImageWidget* mpImage = nullptr;
};