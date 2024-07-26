
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
QT_END_NAMESPACE


class cCreateHyperspectralExperimentFromPlotInfoDlg : public cCreateHyperspectralExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralExperimentFromPlotInfoDlg(const QString& filename, QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralExperimentFromPlotInfoDlg();


private slots:
	bool generate() override;
	void onShowPath() override;
	void onPlotUnitChange(const QString& text);
	void onOffsetUnitChange(const QString& text);

private:
	void createControls_PointSelection() override;
	void createControls_SubScanInfo() override;

	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;
	void createLayout_SubScanInfo(QVBoxLayout* pMainLayout) override;

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

	QComboBox* mpPlotOrientation = nullptr;

	QComboBox* mpPlotUnits = nullptr;

	QRadioButton* mpStart = nullptr;
	QRadioButton* mpCenter = nullptr;
	QRadioButton* mpEnd = nullptr;

	double mOffsetConversionFactor = 1.0;

	QLabel* mpX_OffsetLabel = nullptr;
	QLineEdit* mpX_Offset = nullptr;
	QLabel* mpY_OffsetLabel = nullptr;
	QLineEdit* mpY_Offset = nullptr;

	QComboBox* mpOffsetUnits = nullptr;
};