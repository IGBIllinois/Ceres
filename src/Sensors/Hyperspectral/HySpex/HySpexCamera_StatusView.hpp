
#pragma once

#include "../Sensors/SensorStatusView.hpp"

#include "HySpexDataTypes.hpp"

#include <QTime>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
	class QBoxLayout;
	class QLabel;
	class QLineEdit;
	class QPushButton;
	class QImage;
QT_END_NAMESPACE

// Forward Declaration
class cHySpexCameraModel;
class QButtonIndicator;
class QCustomPlot;


class cHySpexCamera_StatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cHySpexCamera_StatusView(cHySpexCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexCamera_StatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;

signals:
	void requestAutoExposure(bool state);

	void requestCalcBackground();
	void requestStopBackground();

public slots:
	void onInitStatusChange(hyspex::InitStatus status);
	void onCommStatusChange(hyspex::CommunicationStatus status);
	void onAcqStatusChange(hyspex::AcquisitionStatus status);
	void onBgStatusChange(hyspex::BackgroundStatus status);
	void onCoolingStatusChange(hyspex::CoolingStatus status);
	void onShutterStatusChange(hyspex::ShutterStatus status);

	void onAvgFramesChange(std::uint16_t avgFrames);
	void onFramePeriodChange(std::uint32_t period_us);
	void onMinFramePeriodChange(std::uint32_t period_us);
	void onIntegrationTimeChange(std::uint32_t time_us);
	void onMaxIntegrationTimeChange(std::uint32_t time_us);
	void onAmbientTempChange(double temp_C);
	virtual void onSensorTempChange(double temp_C);

	void onLensInfoChange(QString name, double working_distance_cm, double fov_deg);

	void onComputeModeChange();

	void onSaturationDataUpdated();
	void onBandDataUpdated();
	void onFocusDataUpdated(double focus_number);
	void onSpatialDistributionUpdated();
	void onSpectralDistributionUpdated();

	void updateImage(const QImage& image);

protected:
	void doStatusLayout(QBoxLayout* pMainLayout);
	void doAcqStatusLayout(QBoxLayout* pMainLayout);
	void doLensInfoLayout(QBoxLayout* pMainLayout);
	void doPlotLayout(QBoxLayout* pMainLayout);

private slots:
	void saturationButtonToggled(bool state);
	void bandButtonToggled(bool state);
	void focusButtonToggled(bool state);
	void autoExposureButtonToggled(bool state);
	void SpatialDistributionButtonToggled(bool state);
	void SpectralDistributionButtonToggled(bool state);
	void backgroundPressed();

private:
	void unclickAllButtons(QPushButton* pExcept);

protected:
	QButtonIndicator* mpInitializationStatus = nullptr;
	QButtonIndicator* mpCommunicationStatus = nullptr;
	QButtonIndicator* mpAcquisitionStatus = nullptr;
	QButtonIndicator* mpBackgroundStatus = nullptr;
	QButtonIndicator* mpCoolingStatus = nullptr;
	QButtonIndicator* mpShutterStatus = nullptr;

	/*
	 * Acquisition Information
	 */
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

	/*
	 * Lens Information
	 */
	QLabel* mLensNameLabel = nullptr;
	QLineEdit* mpLensName = nullptr;

	QLabel* mLensWorkingDistanceLabel = nullptr;
	QLineEdit* mpLensWorkingDistance_cm = nullptr;

	QLabel* mLensFieldOfViewLabel = nullptr;
	QLineEdit* mpLensFieldOfView_deg = nullptr;

	/*
	 * Various items for showing HySpex data
	 */
	QPushButton* mpPercentSaturationButton = nullptr;
	QPushButton* mpPercentBandButton = nullptr;
	QPushButton* mpFocusButton = nullptr;
	QPushButton* mpAutoExposure = nullptr;
	QPushButton* mpSpatialDistribution = nullptr;
	QPushButton* mpSpectralDistribution = nullptr;
	QPushButton* mpDoBackground = nullptr;

	QCustomPlot* mpPlot = nullptr;
	QVector<qreal> mX;
	QVector<qreal> mY;

	QTime mFocusTimeStart;
	double mMaxFocusValue = 0.0;
	int mFocusLastPointKey_ms = 0;
	int mFocusCounter = 0;

	QLabel* mpCurrentFocusLabel = nullptr;
	QLineEdit* mpCurrentFocus = nullptr;

	QLabel* mpBestFocusLabel = nullptr;
	QLineEdit* mpBestFocus = nullptr;

private:
	cHySpexCameraModel* const mpModel;
};