
#pragma once

#include "ExperimentFile.hpp"

#include <QDialog>
#include <QSharedPointer>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
class QAbstractItemModel;
class QTableView;
class QVBoxLayout;
QT_END_NAMESPACE


class cCreateLidarExperimentDlg : public QDialog
{
	Q_OBJECT

public:
	cCreateLidarExperimentDlg(QWidget* parent = nullptr);
	virtual ~cCreateLidarExperimentDlg();

	void initialize();

signals:
	void clearPaths();
	void drawPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);
	void experimentChanged(QSharedPointer<cExperimentFile> experiment);

protected slots:
	virtual bool generate() = 0;
	virtual void onShowPath() = 0;

private slots:
	void accept() override;
	void onMetaInfoUpdate();
	void onControllerUpdate();
	void onSensorUpdate();
	void onSubScanUnitChange(const QString& text);
	void onHasSubScans(int state);
	void onNumSubScansChanged();

protected:
	void createControls();

	virtual void createControls_TitleInfo();
	virtual void createControls_PointSelection() = 0;
	virtual void createControls_Preamble();
	virtual void createControls_Measurement();
	virtual void createControls_Postamble();
	virtual void createControls_SubScanInfo();

	void createLayout();

	virtual void createLayout_TitleInfo(QVBoxLayout* pMainLayout);
	virtual void createLayout_PointSelection(QVBoxLayout* pMainLayout) = 0;
	virtual void createLayout_Preamble(QVBoxLayout* pMainLayout);
	virtual void createLayout_Measurement(QVBoxLayout* pMainLayout);
	virtual void createLayout_Postamble(QVBoxLayout* pMainLayout);
	virtual void createLayout_SubScanInfo(QVBoxLayout* pMainLayout);

protected:
	double mSubScanConversionFactor = 1.0;

	cExperimentMetaInfo mMetaInfo;
	std::unique_ptr<cExperimentCtrlInfo> mCtrlInfo;
	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensorInfo;

	/* Title Info */
	QLineEdit* mpTitle = nullptr;

	QPushButton* mpMetaInfo = nullptr;
	QPushButton* mpCtrlInfo = nullptr;
	QPushButton* mpSensorInfo = nullptr;

	QPushButton* mpClearPath = nullptr;
	QPushButton* mpShowPath = nullptr;

	/* Measurement Preamble */
	QLineEdit* mpTravelHeight_m = nullptr;
	QLineEdit* mpTravelVerticalSpeed_mmps = nullptr;
	QLineEdit* mpTravelSpeed_mmps = nullptr;

	/* Measurement */
	QLineEdit* mpBeginningOffset_m = nullptr;
	QLineEdit* mpStartMeasurementDelay_sec = nullptr;

	QLineEdit* mpMinIntegrationTime_sec = nullptr;
	QLineEdit* mpMaxIntegrationTime_sec = nullptr;
	QLineEdit* mpErrorThreshold_mm = nullptr;

	QLineEdit* mpMeasurementHeight_m = nullptr;
	QComboBox* mpHeightReference = nullptr;
	QLineEdit* mpMeasurementSpeed_mmps = nullptr;

	QLineEdit* mpEndingOffset_m = nullptr;
	QLineEdit* mpEndMeasurementDelay_sec = nullptr;

	/* Gimble Orientation */
	QLineEdit* mpGimbleTilt_deg = nullptr;
	QLineEdit* mpGimbleRoll_deg = nullptr;
	QLineEdit* mpGimblePan_deg = nullptr;
	QLineEdit* mpSensorOffset_mm = nullptr;

	/* Postamble */
	QLineEdit* mpSafeHeight_m = nullptr;
	QLineEdit* mpSafeVerticalSpeed_mmps = nullptr;

	/* Sub Scan Info */
	QCheckBox* mpHasSubScans = nullptr;
	QLineEdit* mpNumOfScans = nullptr;
	QComboBox* mpSubScanOrientation = nullptr;
	QComboBox* mpSubScanUnits = nullptr;

	QLabel* mpSubScanSeparationLabel = nullptr;
	QLineEdit* mpSubScanSeparation = nullptr;

	QCheckBox* mpFastMode = nullptr;

	QRadioButton* mpScanEveryRow = nullptr;
	QRadioButton* mpScanCenterOnly = nullptr;
	QRadioButton* mpScanInsideRows = nullptr;
};