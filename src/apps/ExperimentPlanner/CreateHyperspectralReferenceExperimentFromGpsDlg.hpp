
#pragma once

#include "CreateHyperspectralReferenceExperimentDlg.hpp"


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


class cCreateHyperspectralReferenceExperimentFromGpsDlg : public cCreateHyperspectralReferenceExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralReferenceExperimentFromGpsDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralReferenceExperimentFromGpsDlg();


private slots:
	bool generate() override;
	void onShowPath() override;
	void onScanUnitChange(const QString& text);

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
};