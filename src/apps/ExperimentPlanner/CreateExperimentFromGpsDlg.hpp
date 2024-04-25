
#pragma once

#include "ExperimentFile.hpp"

#include <QDialog>


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


class cCreateExperimentFromGpsDlg : public QDialog
{
	Q_OBJECT

public:
	cCreateExperimentFromGpsDlg(cExperimentFile& info, const QString& filename, QWidget* parent = nullptr);
	virtual ~cCreateExperimentFromGpsDlg();

signals:
	void clearPaths();
	void drawPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);
	void experimentChanged();
	void saveExperiment();

private slots:
	void accept() override;
	void generate();
	void onMetaInfoUpdate();
	void onControllerUpdate();
	void onSensorUpdate();

	void onShowPath();

private:
	void createControls();
	void createLayout();

private:
	cExperimentFile& mInfo;

	QLineEdit* mpTitle = nullptr;

	QPushButton* mpMetaInfo = nullptr;
	QPushButton* mpCtrlInfo = nullptr;
	QPushButton* mpSensorInfo = nullptr;

	uint32_t mStartIndex = 0;
	uint32_t mEndIndex = 0;

	QTableView* mpStartPosition = nullptr;
	QTableView* mpEndPosition = nullptr;

	QAbstractItemModel* mpModel = nullptr;

	QPushButton* mpClearPath = nullptr;
	QPushButton* mpShowPath = nullptr;

	QCheckBox* mpInverseDirection = nullptr;
	QCheckBox* mpUseIntermediatePoints = nullptr;

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

	QPushButton* mpSaveAs = nullptr;
};