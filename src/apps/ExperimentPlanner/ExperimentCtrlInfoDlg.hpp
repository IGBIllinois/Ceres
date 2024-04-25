
#pragma once

#include "ExperimentFile.hpp"

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QStackedLayout;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
QT_END_NAMESPACE


class cExperimentCtrlInfoDlg : public QDialog
{
	Q_OBJECT

public:
	cExperimentCtrlInfoDlg(cExperimentFile& info, QWidget* parent = nullptr);
	virtual ~cExperimentCtrlInfoDlg();

private slots:
	void accept() override;
	void onControllerChange(const QString& text);

private:
	void createControls();
	void createLayout();

private:
	cExperimentFile& mInfo;

	cExperimentCtrlInfo* mpActiveController = nullptr;

	QComboBox* mpController = nullptr;

	QStackedLayout* mpControllerPanels = nullptr;

	QWidget* mpCP_Dummy = nullptr;

	QWidget* mpCP_SpiderCam = nullptr;

	/** SpiderCam Info**/
	QLineEdit* mpSC_UpdateInterval_ms = nullptr;
	QLineEdit* mpSC_PositionTolerance_cm = nullptr;
};