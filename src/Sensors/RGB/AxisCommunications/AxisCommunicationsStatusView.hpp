
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "QIndicator.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
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

public slots:
	void onCameraIdChange(int id);
	void onFrameRateChange(int rate_fps);
	void onImageSizeChange(int width, int height);

private:
	QLabel* mpCameraIdLabel = nullptr;
	QLineEdit* mpCameraId = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSizes = nullptr;

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;
};