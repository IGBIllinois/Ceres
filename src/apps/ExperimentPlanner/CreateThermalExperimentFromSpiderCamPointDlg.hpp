
#pragma once

#include "CreateThermalExperimentDlg.hpp"

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
QT_END_NAMESPACE


// Forward Declaration
class cScanPointsWidget;


class cCreateThermalExperimentFromSpiderCamDlg : public cCreateThermalExperimentDlg
{
	Q_OBJECT

public:
	cCreateThermalExperimentFromSpiderCamDlg(QWidget* parent = nullptr);
	virtual ~cCreateThermalExperimentFromSpiderCamDlg();


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

	QLineEdit* mpStartX_mm = nullptr;
	QLineEdit* mpStartY_mm = nullptr;

	QLabel* mpScanDistanceLabel = nullptr;
	QLineEdit* mpScanDistance = nullptr;

	QComboBox* mpScanOrientation = nullptr;

	QComboBox* mpScanUnits = nullptr;

	QPushButton* mpSampleXY = nullptr;

	cScanPointsWidget* mpPath = nullptr;

	uint32_t mSpidercamX_mm = 0;
	uint32_t mSpidercamY_mm = 0;
};