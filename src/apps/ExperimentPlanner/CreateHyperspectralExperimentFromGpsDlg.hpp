
#pragma once

#include "CreateHyperspectralExperimentDlg.hpp"


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


class cCreateHyperspectralExperimentFromGpsDlg : public cCreateHyperspectralExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralExperimentFromGpsDlg(const QString& filename, QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralExperimentFromGpsDlg();


private slots:
	bool generate() override;
	void onShowPath() override;
	void onPlotUnitChange(const QString& text);

private:
	void createControls_PointSelection() override;
	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;

private:
	double mPlotConversionFactor = 1.0;

	uint32_t mStartIndex = 0;
	uint32_t mEndIndex = 0;

	QTableView* mpStartPosition = nullptr;
	QTableView* mpEndPosition = nullptr;

	QAbstractItemModel* mpModel = nullptr;

	QCheckBox* mpInverseDirection = nullptr;

	QLabel* mpPlotLengthLabel = nullptr;
	QLineEdit* mpPlotLength = nullptr;

	QLabel* mpAlleyLengthLabel = nullptr;
	QLineEdit* mpAlleyLength = nullptr;

//	QComboBox* mpPlotOrientation = nullptr;

	QComboBox* mpPlotUnits = nullptr;

	QRadioButton* mpStart = nullptr;
	QRadioButton* mpCenter = nullptr;
	QRadioButton* mpEnd = nullptr;
};