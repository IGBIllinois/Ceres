
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


class cReferenceHeightDlg : public QDialog
{
	Q_OBJECT

public:
	cReferenceHeightDlg(QWidget* parent = nullptr);
	virtual ~cReferenceHeightDlg();

	int32_t getReferenceHeight_mm() const;

	void setPosition(int32_t x_mm, int32_t y_mm, int32_t z_mm);
	void setGeometricHeight_mm(int32_t geometric_height_mm);
	void setReferenceHeight_mm(int32_t ref_height_mm);

private slots:
	void accept() override;
	void compute();

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpSpidercamX_mm = nullptr;
	QLineEdit* mpSpidercamY_mm = nullptr;
	QLineEdit* mpSpidercamZ_mm = nullptr;

	QLineEdit* mpGeometricHeight_mm = nullptr;

	QLabel* mpReferenceHeight_mm = nullptr;
};