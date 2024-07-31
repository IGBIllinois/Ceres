
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
class QVBoxLayout;
QT_END_NAMESPACE


class cCreateHyperspectralReferenceExperimentDlg : public QDialog
{
	Q_OBJECT

public:
	cCreateHyperspectralReferenceExperimentDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralReferenceExperimentDlg();

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
	void onControllerUpdate();
	void onSensorUpdate();

protected:
	void createControls();

	virtual void createControls_TitleInfo();
	virtual void createControls_PointSelection() = 0;
	virtual void createControls_Preamble();
	virtual void createControls_Measurement();
	virtual void createControls_Postamble();

	void createLayout();

	virtual void createLayout_TitleInfo(QVBoxLayout* pMainLayout);
	virtual void createLayout_PointSelection(QVBoxLayout* pMainLayout) = 0;
	virtual void createLayout_Preamble(QVBoxLayout* pMainLayout);
	virtual void createLayout_Measurement(QVBoxLayout* pMainLayout);
	virtual void createLayout_Postamble(QVBoxLayout* pMainLayout);

protected:
	std::unique_ptr<cExperimentCtrlInfo> mCtrlInfo;
	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensorInfo;

	/* Title Info */
	QLineEdit* mpTitle = nullptr;

	QPushButton* mpCtrlInfo = nullptr;
	QPushButton* mpSensorInfo = nullptr;

	/* Measurement Preamble */
	QLineEdit* mpTravelHeight_m = nullptr;
	QLineEdit* mpTravelVerticalSpeed_mmps = nullptr;
	QLineEdit* mpTravelSpeed_mmps = nullptr;

	/* Measurement */
	QLineEdit* mpStartMeasurementDelay_sec = nullptr;
	QLineEdit* mpReferenceHeight_m = nullptr;
	QComboBox* mpLensFocalDistance = nullptr;
	QLineEdit* mpMeasurementTime_sec = nullptr;

	/* Gimble Orientation */
	QLineEdit* mpGimbleTilt_deg = nullptr;
	QLineEdit* mpGimbleRoll_deg = nullptr;
	QLineEdit* mpGimblePan_deg = nullptr;
	QLineEdit* mpSensorOffset_mm = nullptr;

	/* Postamble */
	QLineEdit* mpSafeHeight_m = nullptr;
	QLineEdit* mpSafeVerticalSpeed_mmps = nullptr;

	QCheckBox* mpGeneratePlacementExperiment = nullptr;
};