
#pragma once

#include "RappGroundModel.hpp"
#include "RappAerialModel.hpp"

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


class cComputeGroundHeightDlg : public QDialog
{
	Q_OBJECT

public:
	cComputeGroundHeightDlg(const cRappGroundModel& groundData, QWidget* parent = nullptr);
	virtual ~cComputeGroundHeightDlg();

	int32_t getGroundHeight_mm() const;

	void setPosition(int32_t x_mm, int32_t y_mm);

private slots:
	void compute();

private:
	void createControls();
	void createLayout();

private:
	const cRappGroundModel& mGroundData;

	QLineEdit* mpSpidercamX_mm = nullptr;
	QLineEdit* mpSpidercamY_mm = nullptr;

	QLabel* mpGroundHeight_mm = nullptr;
};