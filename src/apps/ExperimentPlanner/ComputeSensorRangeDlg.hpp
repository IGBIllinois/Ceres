
#pragma once

#include "RappGroundModel.hpp"

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


class cComputeSensorRangeDlg : public QDialog
{
	Q_OBJECT

public:
	cComputeSensorRangeDlg(const cRappGroundModel& groundData, QWidget* parent = nullptr);
	virtual ~cComputeSensorRangeDlg();

	int32_t getRange_mm() const;

	void setPosition(int32_t x_mm, int32_t y_mm, int32_t z_mm);
	void setSensorOffset(int32_t offset_mm);

private slots:
	void compute();

private:
	void createControls();
	void createLayout();

private:
	const cRappGroundModel& mGroundData;

	QLineEdit* mpSpidercamX_mm = nullptr;
	QLineEdit* mpSpidercamY_mm = nullptr;
	QLineEdit* mpSpidercamZ_mm = nullptr;

	QLineEdit* mpSensorOffset_mm = nullptr;

	QLabel* mpRange_mm = nullptr;
};