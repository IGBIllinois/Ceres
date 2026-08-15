
#pragma once

#include "../RgbCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QLabel;
class QLineEdit;
class QComboBox;
class QHBoxLayout;
QT_END_NAMESPACE

class cAxisCommunicationsModel;

class cAxisCommunicationsView : public cRgbCameraView
{
	Q_OBJECT

public:
	cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView();

	virtual void initialize();

signals:
	void requestMode(int mode);
	void requestImageSize(int width, int height);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestLapseInterval_ms(uint32_t interval_ms);

public slots:
	void onModeChange(int mode);
	void onImageSizeChange(int width, int height);
	void onFrameRateChange(int rate_fps);
	void onLapseIntervalChange(int interval_ms);

	virtual void imageUpdated(const QImage& image);

protected slots:
	void modeTextChanged(const QString& text);
	void imageSizesTextChanged(const QString& text);
	void frameRateChanged();
	void lapseIntervalChanged();

protected:
	virtual void createWidgets();
	virtual void doControlLayout(QHBoxLayout* pControlLayout);

protected:
    QLabel* mpImageSizeLabel = nullptr;
	QComboBox* mpImageSizes = nullptr;

	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

protected:
	const cAxisCommunicationsModel* mpModel;
};