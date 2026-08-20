
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "RgbImageWidget.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

// Forward Declaration
class cLucidVisionLabsRgbModel;

class cLucidVisionLabsRgbStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbStatusView(cLucidVisionLabsRgbModel* pModel, QWidget* parent = nullptr);
	virtual ~cLucidVisionLabsRgbStatusView();

	void createWidgets() override;
	void doLayout() override;

	void connectToModel() override;

signals:
	void requestImage();

public slots:
	void onCameraIdChange(int id);
	void onFrameRateChange(int rate_fps);
	void onImageSizeChange(int width, int height);
	void imageUpdated(const QImage& image);

	void resizeEvent(QResizeEvent*) override;

private:
	QLabel* mpCameraIdLabel = nullptr;
	QLineEdit* mpCameraId = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QPushButton* mpGrabImage = nullptr;

	cRgbImageWidget* mpImage = nullptr;

private:
	cLucidVisionLabsRgbModel* mpModel = nullptr;
};