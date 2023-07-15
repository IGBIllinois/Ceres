
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "QIndicator.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

// Forward Declaration
class cOusterModel;


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
	QIndicator* mpSensorInfoValid = nullptr;
	QIndicator* mpTimeInfoValid = nullptr;
	QIndicator* mpLidarModeValid = nullptr;
	QIndicator* mpBeamIntrinsicsValid = nullptr;
	QIndicator* mpImuIntrinsicsValid = nullptr;
	QIndicator* mpLidarIntrinsicsValid = nullptr;
	QIndicator* mpDataFormatValid = nullptr;
	QIndicator* mpAzimuthWindowValid = nullptr;

	QLabel*    mpLidarModeLabel = nullptr;
	QLineEdit* mpLidarMode = nullptr;

	QLabel*    mpMinAzimuthAngleLabel = nullptr;
	QLineEdit* mpMinAzimuthAngle_deg = nullptr;
	QLabel*    mpMaxAzimuthAngleLabel = nullptr;
	QLineEdit* mpMaxAzimuthAngle_deg = nullptr;


	cOusterModel* mpModel;
};