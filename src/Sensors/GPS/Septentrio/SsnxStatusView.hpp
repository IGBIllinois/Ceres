
#pragma once

#include "../Sensors/SensorStatusView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
	class QCheckBox;
QT_END_NAMESPACE

// Forward Declaration


class cSsnxStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
    cSsnxStatusView(QWidget* parent = nullptr);
	virtual ~cSsnxStatusView();

	void createWidgets() override;
	void doLayout() override;

public slots:
	void onPvtCartesianStateChange(bool valid);
	void onPvtGeodeticStateChange(bool valid);
	void onPosCovGeodeticStateChange(bool valid);
	void onVelCovGeodeticStateChange(bool valid);
	void onPosProjectedStateChange(bool valid);
	void onReceiverTimeStateChange(bool valid);
	void onRtcmDatumStateChange(bool valid);

private:
	QCheckBox* mpPvtCartesianValid = nullptr;
	QCheckBox* mpPvtGeodeticValid = nullptr;
	QCheckBox* mpPosCovGeodeticValid = nullptr;
	QCheckBox* mpVelCovGeodeticValid = nullptr;
	QCheckBox* mpPosProjectedValid = nullptr;
	QCheckBox* mpReceiverTimeValid = nullptr;
	QCheckBox* mpRtcmDatumValid = nullptr;
};