
#pragma once

#include "../Sensors/SensorStatusView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
	class QCheckBox;
QT_END_NAMESPACE

// Forward Declaration
class cSsnxModel;
class QLedIndicator;
class QButtonIndicator;


class cSsnxStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
    cSsnxStatusView(cSsnxModel* pModel, QWidget* parent = nullptr);
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
	QLedIndicator* mpPvtCartesianValid = nullptr;
	QLedIndicator* mpPvtGeodeticValid = nullptr;
	QLedIndicator* mpPosCovGeodeticValid = nullptr;
	QLedIndicator* mpVelCovGeodeticValid = nullptr;
	QLedIndicator* mpPosProjectedValid = nullptr;
	QLedIndicator* mpReceiverTimeValid = nullptr;
	QLedIndicator* mpRtcmDatumValid = nullptr;
};