
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


class cCreateHyperspectralReferenceExperimentFromSpiderCamDlg : public cCreateHyperspectralReferenceExperimentDlg
{
	Q_OBJECT

public:
	cCreateHyperspectralReferenceExperimentFromSpiderCamDlg(QWidget* parent = nullptr);
	virtual ~cCreateHyperspectralReferenceExperimentFromSpiderCamDlg();


private slots:
	bool generate() override;

private:
	void createControls_PointSelection() override;
	void createLayout_PointSelection(QVBoxLayout* pMainLayout) override;

private:
	QLineEdit* mpRefPosX_mm = nullptr;
	QLineEdit* mpRefPosY_mm = nullptr;
};