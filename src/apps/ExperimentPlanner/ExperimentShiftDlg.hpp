
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


class cExperimentShiftDlg : public QDialog
{
public:
	cExperimentShiftDlg(QWidget* parent = nullptr);
	virtual ~cExperimentShiftDlg();

	int xShift_mm() const;
	int yShift_mm() const;
	int zShift_mm() const;

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpX_Shift_mm;
	QLineEdit* mpY_Shift_mm;
	QLineEdit* mpZ_Shift_mm;
};