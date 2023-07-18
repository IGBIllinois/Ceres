
#pragma once

#include "../Sensors/SensorStatusView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

// Forward Declaration
class cOusterModel;
class QLedIndicator;


class cOusterStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
    cOusterStatusView(cOusterModel* pModel, QWidget* parent = nullptr);
	virtual ~cOusterStatusView();

	void createWidgets() override;
	void doLayout() override;

public slots:
	void onSensorInfoUpdated();
	void onTimeInfoUpdated();
	void onLidarModeUpdated();
	void onBeamIntrinsicsUpdated();
	void onImuIntrinsicsUpdated();
	void onLidarIntrinsicsUpdated();
	void onDataFormatUpdated();
	void onAzimuthWindowUpdated();


private:
	QLedIndicator* mpSensorInfoValid = nullptr;
	QLedIndicator* mpTimeInfoValid = nullptr;
	QLedIndicator* mpLidarModeValid = nullptr;
	QLedIndicator* mpBeamIntrinsicsValid = nullptr;
	QLedIndicator* mpImuIntrinsicsValid = nullptr;
	QLedIndicator* mpLidarIntrinsicsValid = nullptr;
	QLedIndicator* mpDataFormatValid = nullptr;
	QLedIndicator* mpAzimuthWindowValid = nullptr;

	QLabel*    mpLidarModeLabel = nullptr;
	QLineEdit* mpLidarMode = nullptr;

	QLabel*    mpMinAzimuthAngleLabel = nullptr;
	QLineEdit* mpMinAzimuthAngle_deg = nullptr;
	QLabel*    mpMaxAzimuthAngleLabel = nullptr;
	QLineEdit* mpMaxAzimuthAngle_deg = nullptr;


	cOusterModel* mpModel;
};