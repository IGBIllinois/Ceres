
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


class cComputeSpidercamHeightDlg : public QDialog
{
	Q_OBJECT

public:
	cComputeSpidercamHeightDlg(const cRappGroundModel& groundData, QWidget* parent = nullptr);
	virtual ~cComputeSpidercamHeightDlg();

	int32_t getHeight_mm() const;

	void setPosition(int32_t x_mm, int32_t y_mm);
	void setSensorOffset(int32_t offset_mm);
	void setDesiredHeight_agl(int32_t height_mm);

private slots:
	void compute();

private:
	void createControls();
	void createLayout();

private:
	const cRappGroundModel& mGroundData;

	QLineEdit* mpSpidercamX_mm = nullptr;
	QLineEdit* mpSpidercamY_mm = nullptr;

	QLineEdit* mpSensorOffset_mm = nullptr;

	QLineEdit* mpHeight_mm = nullptr;

	QLabel* mpSpidercamZ_mm = nullptr;
};