
#pragma once

#include "ExperimentCtrlInfo.hpp"

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
	cExperimentCtrlInfoDlg(cExperimentCtrlInfo* info, QWidget* parent = nullptr);
	virtual ~cExperimentCtrlInfoDlg();

	std::unique_ptr<cExperimentCtrlInfo> getControllerInfo() const;

private slots:
	void accept() override;
	void onControllerChange(const QString& text);

private:
	void createControls();
	void createLayout();

private:
	std::unique_ptr<cExperimentCtrlInfo> mInfo = nullptr;

	QComboBox* mpController = nullptr;

	QStackedLayout* mpControllerPanels = nullptr;

	QWidget* mpCP_Dummy = nullptr;

	QWidget* mpCP_SpiderCam = nullptr;

	/** SpiderCam Info**/
	QLineEdit* mpSC_UpdateInterval_ms = nullptr;
	QLineEdit* mpSC_PositionTolerance_cm = nullptr;
};