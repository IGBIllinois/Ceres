
#pragma once

#include "ExperimentMetaInfo.hpp"

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
QT_END_NAMESPACE


class cExperimentMetaInfoDlg : public QDialog
{
public:
	cExperimentMetaInfoDlg(cExperimentMetaInfo& info, QWidget* parent = nullptr);
	virtual ~cExperimentMetaInfoDlg();

	std::string getExperimentTitle() const;
	void setExperimentTitle(const std::string& title);

	std::string getMeasurementTitle() const;
	void setMeasurementTitle(const std::string& title);

private slots:
	void accept() override;
	void apply();

private:
	void createControls();
	void createLayout();

private:
	cExperimentMetaInfo& mInfo;

	QLineEdit* mpExperimentTitle = nullptr;
	QLineEdit* mpMeasurementTitle = nullptr;

	QLineEdit* mpPrincipalInvestigator = nullptr;
	QPlainTextEdit* mpResearchers = nullptr;
	QPlainTextEdit* mpComments = nullptr;

	QLineEdit* mpSpecies = nullptr;
	QLineEdit* mpCultivar = nullptr;
	QPlainTextEdit* mpEvents = nullptr;
	QLineEdit* mpConstructName = nullptr;
	QPlainTextEdit* mpTreatments = nullptr;

	QLineEdit* mpFieldDesign = nullptr;

	QComboBox* mpPlantingMonth = nullptr;
	QLineEdit* mpPlantingDay = nullptr;
	QComboBox* mpPlantingYear = nullptr;

	QComboBox* mpTargetHarvestMonth = nullptr;
	QLineEdit* mpTargetHarvestDay = nullptr;
	QComboBox* mpTargetHarvestYear = nullptr;

	QLineEdit* mpPermitInfo = nullptr;
};