
#pragma once

#include "../Sensors/SensorStatusView.hpp"

#include "../Sensors/GPS/GpsTypes.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCheckBox;
class QLineEdit;
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
	void onSolutionTypeChange(int solution_type);
	void onPositionChange(int x_mm, int y_mm, int z_mm);
	void onGeodeticPVT_Change(double timestamp_s,
		double lat_rad, double lng_rad, double height_m,
		double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
		double groundTrack_deg, int datum, int num_sv, int num_bases);

	void onUTC_Change(int hour, int min, int sec, int day, int month, int year);
	void onReferenceChange(int x_mm, int y_mm, int z_mm, double error_mm, int count);

private:
	QLedIndicator* mpPvtCartesianValid = nullptr;
	QLedIndicator* mpPvtGeodeticValid = nullptr;
	QLedIndicator* mpPosCovGeodeticValid = nullptr;
	QLedIndicator* mpVelCovGeodeticValid = nullptr;
	QLedIndicator* mpPosProjectedValid = nullptr;
	QLedIndicator* mpReceiverTimeValid = nullptr;
	QLedIndicator* mpRtcmDatumValid = nullptr;

	QLineEdit* mpLatitude_deg = nullptr;
	QLineEdit* mpLongitude_deg = nullptr;
	QLineEdit* mpHeight_m = nullptr;

	QLineEdit* mpNorthVelocity_mps = nullptr;
	QLineEdit* mpEastVelocity_mps = nullptr;
	QLineEdit* mpUpVelocity_mps = nullptr;

	QLineEdit* mpGroundTrack_deg = nullptr;

	QLineEdit* mpDatum = nullptr;

	QLineEdit* mpNumBases = nullptr;
	QLineEdit* mpNumSV = nullptr;

	QLineEdit* mpSolutionType = nullptr;

	QLineEdit* mpTimestamp_s = nullptr;

	QLineEdit* mpDate = nullptr;
	QLineEdit* mpTime = nullptr;

	QLineEdit* mpX_mm = nullptr;
	QLineEdit* mpY_mm = nullptr;
	QLineEdit* mpZ_mm = nullptr;

	QLineEdit* mpRef_X_mm = nullptr;
	QLineEdit* mpRef_Y_mm = nullptr;
	QLineEdit* mpRef_Z_mm = nullptr;
	QLineEdit* mpRef_Error_mm = nullptr;
	QLineEdit* mpRef_Count = nullptr;
};