
#pragma once

#include "CreateLidarExperimentDlg.hpp"


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


class cCreateLidarExperimentFromPlotInfoDlg : public cCreateLidarExperimentDlg
{
	Q_OBJECT

public:
	cCreateLidarExperimentFromPlotInfoDlg(const QString& filename, QWidget* parent = nullptr);
	virtual ~cCreateLidarExperimentFromPlotInfoDlg();

private slots:
	bool generate();
	void onUnitChange(const QString& text);

	void onShowPath();

private:
	void createControls_PointSelection() override;
	void createControls_SubScanInfo() override;

	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;
	void createLayout_SubScanInfo(QVBoxLayout* pMainLayout) override;

private:
	double mConversionFactor = 1.0;

	uint32_t mStartIndex = 0;
//BAF	uint32_t mEndIndex = 0;

	QTableView* mpStartPosition = nullptr;
//BAF	QTableView* mpEndPosition = nullptr;

	QRadioButton* mpPointAtStart = nullptr;
	QRadioButton* mpPointAtCenter = nullptr;
	QRadioButton* mpPointAtEnd = nullptr;

	QAbstractItemModel* mpModel = nullptr;

//BAF	QCheckBox* mpInverseDirection = nullptr;

	QLabel* mpPlotLengthLabel = nullptr;
	QLineEdit* mpPlotLength = nullptr;

	QComboBox* mpPlotOrientation = nullptr;
	QComboBox* mpUnits = nullptr;
};