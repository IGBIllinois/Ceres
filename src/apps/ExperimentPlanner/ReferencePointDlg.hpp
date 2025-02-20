
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE


class cReferencePointDlg : public QDialog
{
public:
	cReferencePointDlg(QWidget* parent = nullptr);
	virtual ~cReferencePointDlg();

	double minIntegrationTime_sec() const;
	double maxIntegrationTime_sec() const;

	int errorThreshold_mm() const;

	void setMinIntegrationTime_sec(double secs);
	void setMaxIntegrationTime_sec(double secs);
	void setErrorThreshold_mm(int threshold_mm);

private:
	void createControls();
	void createLayout();

private:

	QLineEdit* mpMinIntegrationTime_sec;
	QLineEdit* mpMaxIntegrationTime_sec;
	QLineEdit* mpErrorThreshold_mm;
};