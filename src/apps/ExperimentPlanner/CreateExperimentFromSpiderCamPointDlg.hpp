
#pragma once

#include "ExperimentFile.hpp"

#include <QDialog>
#include <QSharedPointer>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
class QAbstractItemModel;
class QTableView;
QT_END_NAMESPACE


class cCreateExperimentFromSpiderCamDlg : public QDialog
{
	Q_OBJECT

public:
	cCreateExperimentFromSpiderCamDlg(QWidget* parent = nullptr);
	virtual ~cCreateExperimentFromSpiderCamDlg();

signals:
	void clearPaths();
	void drawPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);
	void experimentChanged(QSharedPointer<cExperimentFile> experiment);

private slots:
	void accept() override;
	void generate();
	void onMetaInfoUpdate();
	void onControllerUpdate();
	void onSensorUpdate();
	void onScanUnitChange(const QString& text);
	void onSubScanUnitChange(const QString& text);
	void onHasSubScans(int state);

	void onShowPath();

private:
	void createControls();
	void createLayout();

private:
	double mScanConversionFactor = 1.0;
	double mSubScanConversionFactor = 1.0;

	cExperimentMetaInfo mMetaInfo;
	std::unique_ptr<cExperimentCtrlInfo> mCtrlInfo;
	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensorInfo;

	QLineEdit* mpTitle = nullptr;

	QPushButton* mpMetaInfo = nullptr;
	QPushButton* mpCtrlInfo = nullptr;
	QPushButton* mpSensorInfo = nullptr;

	QLineEdit* mpStartX_mm = nullptr;
	QLineEdit* mpStartY_mm = nullptr;

	QLabel* mpScanDistanceLabel = nullptr;
	QLineEdit* mpScanDistance = nullptr;

	QComboBox* mpScanOrientation = nullptr;

	QComboBox* mpScanUnits = nullptr;

	QPushButton* mpClearPath = nullptr;
	QPushButton* mpShowPath = nullptr;

	QLineEdit* mpTravelHeight_m = nullptr;
	QLineEdit* mpTravelVerticalSpeed_mmps = nullptr;
	QLineEdit* mpTravelSpeed_mmps = nullptr;

	QLineEdit* mpBeginningOffset_m = nullptr;
	QLineEdit* mpEndingOffset_m = nullptr;

	QLineEdit* mpStartMeasurementDelay_sec = nullptr;
	QLineEdit* mpMeasurementHeight_m = nullptr;
	QLineEdit* mpMeasurementSpeed_mmps = nullptr;
	QLineEdit* mpEndMeasurementDelay_sec = nullptr;

	QLineEdit* mpSafeHeight_m = nullptr;
	QLineEdit* mpSafeVerticalSpeed_mmps = nullptr;

	QCheckBox* mpHasSubScans = nullptr;
	QLineEdit* mpNumOfScans = nullptr;
	QComboBox* mpSubScanOrientation = nullptr;
	QComboBox* mpSubScanUnits = nullptr;

	QLabel* mpSubScanSeparationLabel = nullptr;
	QLineEdit* mpSubScanSeparation = nullptr;

	QCheckBox* mpFastMode = nullptr;
};