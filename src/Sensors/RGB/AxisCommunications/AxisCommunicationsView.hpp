
#pragma once

#include "../RgbCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QLabel;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class cAxisCommunicationsModel;

class cAxisCommunicationsView : public cRgbCameraView
{
	Q_OBJECT

public:
	cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView();

	virtual void initialize();

public slots:
	void onImageSizeChange(int width, int height);

	void onModeChange(int mode);
	void onFrameRateChange(int rate_fps);
	void onLapseIntervalChange(int interval_ms);

	virtual void imageUpdated(const QImage& image);

protected:
    QLabel* mpImageSizeLabel = nullptr;
    QLineEdit* mpImageSize = nullptr;

	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

protected:
	const cAxisCommunicationsModel* mpModel;
};