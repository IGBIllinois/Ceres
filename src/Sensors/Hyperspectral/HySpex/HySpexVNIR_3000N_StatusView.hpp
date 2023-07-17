
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "QIndicator.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Forward Declaration
class cHySpexVNIR_3000N_Model;


class cHySpexVNIR_3000N_StatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cHySpexVNIR_3000N_StatusView(cHySpexVNIR_3000N_Model* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexVNIR_3000N_StatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;
	void doLayout() override;

public slots:
	void onAvgFramesChange(std::uint16_t avgFrames);
	void onFramePeriodChange(std::uint32_t period_us);
	void onMinFramePeriodChange(std::uint32_t period_us);
	void onIntegrationTimeChange(std::uint32_t time_us);
	void onMaxIntegrationTimeChange(std::uint32_t time_us);
	void onAmbientTempChange(double temp_C);
	void onSensorTempChange(double temp_C);

protected:
	QLabel* mpAvgFramesLabel = nullptr;
	QLineEdit* mpAvgFrames = nullptr;

	QLabel* mpFramePeriodLabel = nullptr;
	QLineEdit* mpFramePeriod_us = nullptr;

	QLabel* mpMinFramePeriodLabel = nullptr;
	QLineEdit* mpMinFramePeriod_us = nullptr;

	QLabel* mpIntegrationTimeLabel = nullptr;
	QLineEdit* mpIntegrationTime_us = nullptr;

	QLabel* mpMaxIntegrationTimeLabel = nullptr;
	QLineEdit* mpMaxIntegrationTime_us = nullptr;

	QLabel* mpAmbientTempLabel = nullptr;
	QLineEdit* mpAmbientTemp_C = nullptr;

	QLabel* mpSensorTempLabel = nullptr;
	QLineEdit* mpSensorTemp_C = nullptr;
};