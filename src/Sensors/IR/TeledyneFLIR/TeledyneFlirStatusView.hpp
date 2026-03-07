
#pragma once

#include "../Sensors/SensorStatusView.hpp"
//#include "RgbImageWidget.hpp"

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
	void requestImage();

public slots:
	void onModeChange(int mode);
	void onFrameIntervalChange(int interval_ms);
	void onFrameRateChange(double rate_fps);
	void onImageSizeChange(int width, int height);
	void imageUpdated(const QImage& image);

	void resizeEvent(QResizeEvent*) override;

private:
	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpFrameIntervalLabel = nullptr;
	QLineEdit* mpFrameInterval_s = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QPushButton* mpGrabImage = nullptr;
};