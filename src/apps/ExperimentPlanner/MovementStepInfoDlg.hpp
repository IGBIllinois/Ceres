
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE


class cMovementStepInfoDlg : public QDialog
{
public:
	cMovementStepInfoDlg(QWidget* parent = nullptr);
	virtual ~cMovementStepInfoDlg();

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpPlotName;
	QLineEdit* mpDescription;
	QLineEdit* mpSpecies;
	QLineEdit* mpCultivar;

	QLineEdit* mpEvent;
	QLineEdit* mpConstructName;
	QLineEdit* mpPotLabel;
	QLineEdit* mpSeedGeneration;
	QLineEdit* mpCopyNumber;
};