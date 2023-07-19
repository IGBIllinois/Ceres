
#pragma once

#include "../Sensors/SensorStatusView.hpp"

#include "HySpexDataTypes.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
	class QBoxLayout;
	class QLabel;
	class QLineEdit;
QT_END_NAMESPACE

// Forward Declaration
class cHySpexCameraModel;
class QButtonIndicator;


class cHySpexStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cHySpexStatusView(cHySpexCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexStatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;

public slots:
	void onInitStatusChange();
	void onCommStatusChange();
	void onAcqStatusChange();
	void onBgStatusChange();
	void onCoolingStatusChange();
	void onShutterStatusChange();

	void onLensInfoChange();

protected:
	void doStatusLayout(QBoxLayout* pMainLayout);
	void doLensInfoLayout(QBoxLayout* pMainLayout);


protected:
	QButtonIndicator* mpInitializationStatus = nullptr;
	QButtonIndicator* mpCommunicationStatus = nullptr;
	QButtonIndicator* mpAcquisitionStatus = nullptr;
	QButtonIndicator* mpBackgroundStatus = nullptr;
	QButtonIndicator* mpCoolingStatus = nullptr;
	QButtonIndicator* mpShutterStatus = nullptr;

	QLabel* mLensNameLabel = nullptr;
	QLineEdit* mpLensName = nullptr;

	QLabel* mLensWorkingDistanceLabel = nullptr;
	QLineEdit* mpLensWorkingDistance_cm = nullptr;

	QLabel* mLensFieldOfViewLabel = nullptr;
	QLineEdit* mpLensFieldOfView_deg = nullptr;

private:
	const cHySpexCameraModel* mpModel;
};