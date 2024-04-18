
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

	bool hasX() const;
	int  x_mm() const;

	bool hasY() const;
	int  y_mm() const;

	bool hasZ() const;
	int  z_mm() const;

	int speed_mmps() const;

	bool   hasPan() const;
	double pan_deg() const;

	bool   hasTilt() const;
	double tilt_deg() const;

	bool   hasRoll() const;
	double roll_deg() const;

	bool recording() const;

	void setX_mm(int x_mm);
	void setY_mm(int y_mm);
	void setZ_mm(int z_mm);

	void setSpeed_mmps(int speed_mmps);

	void setPan_deg(double pan_deg);
	void setTilt_deg(double tilt_deg);
	void setRoll_deg(double roll_deg);

	void setRecording(bool recording);

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpX_mm;
	QLineEdit* mpY_mm;
	QLineEdit* mpZ_mm;

	QLineEdit* mpSpeed_mmps;

	QLineEdit* mpPan_deg;
	QLineEdit* mpTilt_deg;
	QLineEdit* mpRoll_deg;

	QCheckBox* mpRecord;
};