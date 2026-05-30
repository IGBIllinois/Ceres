
#pragma once

#include "CreateHyperspectralExperimentDlg.hpp"

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


class cCreateHyperspectralExperimentFromSpiderCamDlg : public cCreateHyperspectralExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralExperimentFromSpiderCamDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralExperimentFromSpiderCamDlg();

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private slots:
	bool generate() override;
	void onShowPath() override;
	void onPlotUnitChange(const QString& text);
	void recordXY();

private:
	void createControls_PointSelection() override;
	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;

private:
	double mPlotConversionFactor = 1.0;

	QLineEdit* mpStartX_mm = nullptr;
	QLineEdit* mpStartY_mm = nullptr;

	QLabel* mpPlotLengthLabel = nullptr;
	QLineEdit* mpPlotLength = nullptr;

	QLabel* mpAlleyLengthLabel = nullptr;
	QLineEdit* mpAlleyLength = nullptr;

	QComboBox* mpPlotOrientation = nullptr;

	QComboBox* mpPlotUnits = nullptr;

	QLineEdit* mpNumOfPlots = nullptr;

	QRadioButton* mpStart = nullptr;
	QRadioButton* mpCenter = nullptr;
	QRadioButton* mpEnd = nullptr;

	QPushButton* mpSampleXY = nullptr;

	uint32_t mSpidercamX_mm = 0;
	uint32_t mSpidercamY_mm = 0;
};