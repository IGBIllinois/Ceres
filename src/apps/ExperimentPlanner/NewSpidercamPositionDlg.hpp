
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


class cNewSpidercam_X_PositionDlg : public QDialog
{
public:
	cNewSpidercam_X_PositionDlg(QWidget* parent = nullptr);
	virtual ~cNewSpidercam_X_PositionDlg();

	int x_mm() const;

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpX_mm;
};

class cNewSpidercam_Y_PositionDlg : public QDialog
{
public:
	cNewSpidercam_Y_PositionDlg(QWidget* parent = nullptr);
	virtual ~cNewSpidercam_Y_PositionDlg();

	int y_mm() const;

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpY_mm;
};

class cNewSpidercam_Z_PositionDlg : public QDialog
{
public:
	cNewSpidercam_Z_PositionDlg(QWidget* parent = nullptr);
	virtual ~cNewSpidercam_Z_PositionDlg();

	int z_mm() const;

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpZ_mm;
};

