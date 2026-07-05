
#pragma once

#include <QDialog>

#include <spidercam_connect/spidercam_com.hpp>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QLabel;
QT_END_NAMESPACE


class cMovementStepInfoDlg : public QDialog
{
	Q_OBJECT

public:
	cMovementStepInfoDlg(QWidget* parent = nullptr);
	virtual ~cMovementStepInfoDlg();

	bool hasX() const;
	int  x_mm() const;

	bool hasY() const;
	int  y_mm() const;

	bool hasZ() const;
	int  z_mm() const;

	bool hasHeightAGL() const;
	int  height_agl_mm() const;

	bool hasHeightACL() const;
	int  height_acl_mm() const;

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

	void setHeightAGL_mm(int height_agl_mm);
	void setHeightACL_mm(int height_acl_mm);

	void setSpeed_mmps(int speed_mmps);

	void setPan_deg(double pan_deg);
	void setTilt_deg(double tilt_deg);
	void setRoll_deg(double roll_deg);

	void setRecording(bool recording);

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private:
	void createControls();
	void createLayout();

private slots:
	void recordXY();
	void recordXYZ();

private:
	QLineEdit* mpX_mm = nullptr;
	QLineEdit* mpY_mm = nullptr;

	QLabel* mpHeightLabel = nullptr;
	QComboBox* mpHeightType = nullptr;

	QLineEdit* mpZ_mm = nullptr;
	QLineEdit* mpHeightAGL_mm = nullptr;
	QLineEdit* mpHeightACL_mm = nullptr;

	QLineEdit* mpSpeed_mmps = nullptr;

	QLineEdit* mpPan_deg = nullptr;
	QLineEdit* mpTilt_deg = nullptr;
	QLineEdit* mpRoll_deg = nullptr;

	QCheckBox* mpRecord = nullptr;

	QPushButton* mpSampleXY = nullptr;
	QPushButton* mpSampleXYZ = nullptr;

	uint32_t mSpidercamX_mm = 0;
	uint32_t mSpidercamY_mm = 0;
	uint32_t mSpidercamZ_mm = 0;
};