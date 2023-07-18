
#pragma once

#include "../Sensors/SensorStatusView.hpp"

#include <hyspex/datatypes.h>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
	class QBoxLayout;
QT_END_NAMESPACE

// Forward Declaration
class QButtonIndicator;


class cHySpexStatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cHySpexStatusView(cSensorModel* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexStatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;

public slots:
	void onAcqStatusChange(hyspex::AcquisitionStatus status);
	void onBgStatusChange(hyspex::BackgroundStatus status);
	void onCommStatusChange(hyspex::CommunicationStatus status);
	void onCoolingStatusChange(hyspex::CoolingStatus status);
	void onShutterStatusChange(hyspex::ShutterStatus status);


protected:
	void doSubLayout(QBoxLayout* pMainLayout);


protected:
	QButtonIndicator* mpAcquisitionStatus = nullptr;
	QButtonIndicator* mpBackgroundStatus = nullptr;
	QButtonIndicator* mpCommunicationStatus = nullptr;
	QButtonIndicator* mpCoolingStatus = nullptr;
	QButtonIndicator* mpShutterStatus = nullptr;
};