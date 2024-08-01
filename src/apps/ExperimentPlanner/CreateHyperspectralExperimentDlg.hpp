
#pragma once

#include "ExperimentFile.hpp"

#include <QDialog>
#include <QSharedPointer>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QGroupBox;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
class QAbstractItemModel;
class QTableView;
class QVBoxLayout;
class QRadioButton;
QT_END_NAMESPACE


class cCreateHyperspectralExperimentDlg : public QDialog
{
	Q_OBJECT

public:
	cCreateHyperspectralExperimentDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralExperimentDlg();

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
	void onScanUnitChange(const QString& text);
	void onAskHeightOffset(int state);
	void onSubScanUnitChange(const QString& text);
	void onHasSubScans(int state);
	void onNumSubScansChanged();

protected:
	enum class eSubScanOrientation {NORTH_TO_SOUTH, SOUTH_TO_NORTH, EAST_TO_WEST, WEST_TO_EAST};
	eSubScanOrientation getSubScanOrientation() const;

	bool isScanDistanceValid() const;
	int getScanDistance_mm() const;

	bool isSubScanDistanceValid() const;
	int getSubScanSeparation_mm() const;

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
	QLineEdit* mpStartMeasurementDelay_sec = nullptr;
	QLineEdit* mpHeightOffset = nullptr;
	QCheckBox* mpAskForOffset = nullptr;
	QComboBox* mpLensFocalDistance = nullptr;
	QLineEdit* mpMeasurementSpeed_mmps = nullptr;
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

	QCheckBox* mpFastMode = nullptr;

	QGroupBox*    mpScanOptions = nullptr;
	QRadioButton* mpScanEveryRow = nullptr;
	QRadioButton* mpScanCenterOnly = nullptr;
	QRadioButton* mpScanInsideRows = nullptr;

private:
	double mScanConversionFactor = 1.0;
	double mSubScanConversionFactor = 1.0;

	QLabel* mpScanDistanceLabel = nullptr;
	QLineEdit* mpScanDistance = nullptr;
	QComboBox* mpScanUnits = nullptr;

	QLabel* mpSubScanSeparationLabel = nullptr;
	QLineEdit* mpSubScanSeparation = nullptr;
	QComboBox* mpSubScanUnits = nullptr;

	QComboBox* mpSubScanOrientation = nullptr;
};