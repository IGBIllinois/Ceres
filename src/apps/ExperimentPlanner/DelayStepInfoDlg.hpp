
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


class cDelayStepInfoDlg : public QDialog
{
public:
	cDelayStepInfoDlg(QWidget* parent = nullptr);
	virtual ~cDelayStepInfoDlg();

	bool hasHours() const;
	int  hours() const;

	bool hasMinutes() const;
	int  minutes() const;

	double seconds() const;

	bool recording() const;

	void setHours(int hours);
	void setMinutes(int minutes);
	void setSeconds(double secs);
	void setRecording(bool recording);

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpHours;
	QLineEdit* mpMinutes;
	QLineEdit* mpSeconds;

	QCheckBox* mpRecord;
};