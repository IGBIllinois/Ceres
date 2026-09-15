
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "ThermalImageWidget.hpp"

#include "../ThermalColorTypes.hpp"


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

signals:
	void requestMode(int mode);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestLapseInterval_ms(uint32_t interval_ms);

	void requestColorPalette(eColorTable palette);

	void requestImage();
	void requestImages(bool update_view);

public slots:
	void onModeChange(int mode);
	void onLapseIntervalChange(int interval_ms);
	void onFrameRateChange(double rate_fps);
	void onImageSizeChange(int width, int height);
	void thermalRangeUpdated(float minValue_K, float maxValue_K);
	void colorPaletteUpdated(eColorTable palette);
	void imageUpdated(const QImage& image);

	void resizeEvent(QResizeEvent*) override;

protected slots:
	void modeTextChanged(const QString&);
	void colorPaletteTextChanged(const QString&);
	void frameRateEditingFinished();
	void lapseIntervalEditingFinished();
	void grabImageRequested();

private:
	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QLabel* mpThermalRangeLabel = nullptr;
	QLineEdit* mpThermalRange = nullptr;

	QComboBox* mpColorPalette = nullptr;

	QPushButton* mpGrabImage = nullptr;

	cThermalImageWidget* mpThermalImage = nullptr;
};