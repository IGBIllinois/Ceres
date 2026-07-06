
#pragma once

#include "CreateHyperspectralReferenceExperimentDlg.hpp"

#include <spidercam_connect/spidercam_types.hpp>


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
class QGroupBox;
QT_END_NAMESPACE

// Forward Declaration
class cScanPointsWidget;


class cCreateHyperspectralReferenceExperimentFromSpiderCamDlg : public cCreateHyperspectralReferenceExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralReferenceExperimentFromSpiderCamDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralReferenceExperimentFromSpiderCamDlg();

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private slots:
	bool generate() override;
	void onShowPath() override;
	void onScanUnitChange(const QString& text);
	void recordXY();

private:
	void createControls_PointSelection() override;
	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;

private:
	double mScanConversionFactor = 1.0;

	QLineEdit* mpRefPosX_mm = nullptr;
	QLineEdit* mpRefPosY_mm = nullptr;

	QLabel* mpScanDistanceLabel = nullptr;
	QLineEdit* mpScanDistance = nullptr;

	QComboBox* mpScanOrientation = nullptr;

	QComboBox* mpScanUnits = nullptr;

	QPushButton* mpSampleXY = nullptr;

	uint32_t mSpidercamX_mm = 0;
	uint32_t mSpidercamY_mm = 0;

	cScanPointsWidget* mpPath = nullptr;

};