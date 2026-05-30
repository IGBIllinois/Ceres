
#pragma once

#include <QDialog>

#include <spidercam_connect/spidercam_types.hpp>

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
	cNewSpidercam_X_PositionDlg(int minX_mm, int maxX_mm, QWidget* parent = nullptr);
	virtual ~cNewSpidercam_X_PositionDlg();

	int x_mm() const;

	void setX_mm(int x_mm);

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private:
	void createControls(int minX_mm, int maxX_mm);
	void createLayout();

private slots:
	void recordX();

private:
	QLineEdit*   mpX_mm = nullptr;
	QPushButton* mpSampleX = nullptr;

	uint32_t mSpidercamX_mm = 0;
};

class cNewSpidercam_Y_PositionDlg : public QDialog
{
public:
	cNewSpidercam_Y_PositionDlg(int minY_mm, int maxY_mm, QWidget* parent = nullptr);
	virtual ~cNewSpidercam_Y_PositionDlg();

	int y_mm() const;

	void setY_mm(int y_mm);

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private:
	void createControls(int minY_mm, int maxY_mm);
	void createLayout();

private slots:
	void recordY();

private:
	QLineEdit*   mpY_mm = nullptr;
	QPushButton* mpSampleY = nullptr;

	uint32_t mSpidercamY_mm = 0;
};

class cNewSpidercam_Z_PositionDlg : public QDialog
{
public:
	cNewSpidercam_Z_PositionDlg(int minZ_mm, int maxZ_mm, QWidget* parent = nullptr);
	virtual ~cNewSpidercam_Z_PositionDlg();

	int z_mm() const;

	void setZ_mm(int z_mm);

public slots:
	void positionUpdated(spidercam::sPosition_1_t pos);

private:
	void createControls(int minZ_mm, int maxZ_mm);
	void createLayout();

private slots:
	void recordZ();

private:
	QLineEdit*   mpZ_mm = nullptr;
	QPushButton* mpSampleZ = nullptr;

	uint32_t mSpidercamZ_mm = 0;
};

