
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
QT_END_NAMESPACE


class cExperimentMetaInfoDlg : public QDialog
{
public:
	cExperimentMetaInfoDlg(QWidget* parent = nullptr);
	virtual ~cExperimentMetaInfoDlg();

private slots:
	void accept() override;
	void reject() override;
	void apply();

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpPrincipalInvestigator = nullptr;
	QPlainTextEdit* mpResearchers = nullptr;
	QPlainTextEdit* mpComments = nullptr;

	QLineEdit* mpSpecies = nullptr;
	QLineEdit* mpCultivar = nullptr;
	QLineEdit* mpEvents = nullptr;
	QLineEdit* mpConstructName = nullptr;
	QPlainTextEdit* mpTreatments = nullptr;

	QLineEdit* mpFieldDesign = nullptr;

	QLineEdit* mpPlantingDate = nullptr;
	QLineEdit* mpTargetHarvestDate = nullptr;

	QLineEdit* mpPermitInfo = nullptr;
};