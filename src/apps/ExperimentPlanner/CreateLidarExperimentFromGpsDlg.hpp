
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


class cCreateLidarExperimentFromGpsDlg : public cCreateLidarExperimentDlg
{
	Q_OBJECT

public:
	cCreateLidarExperimentFromGpsDlg(const QString& filename, QWidget* parent = nullptr);
	virtual ~cCreateLidarExperimentFromGpsDlg();


private slots:
	bool generate() override;
	void onShowPath() override;

private:
	void createControls_PointSelection() override;
	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;

private:
	uint32_t mStartIndex = 0;
	uint32_t mEndIndex = 0;

	QTableView* mpStartPosition = nullptr;
	QTableView* mpEndPosition = nullptr;

	QAbstractItemModel* mpModel = nullptr;

	QCheckBox* mpInverseDirection = nullptr;
};