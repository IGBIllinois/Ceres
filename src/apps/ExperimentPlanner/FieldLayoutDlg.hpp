
#pragma once

#include "Spidercam/SpidercamScanArea.hpp"

#include <QDialog>
#include <QColor>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE


class cFieldLayoutDlg : public QDialog
{
public:
	cFieldLayoutDlg(QWidget* parent = nullptr);
	virtual ~cFieldLayoutDlg();

	const cSpidercamScanArea::experimentLayout_t& getLayout() const;

	void setDefaults(const cSpidercamScanArea::experimentLayout_t& layout);

private slots:
	void accept() override;

private:
	void createControls();
	void createLayout();

private:
	cSpidercamScanArea::experimentLayout_t mLayout;

	QLineEdit* mpCaption = nullptr;
	QLineEdit* mpFontSize = nullptr;
	QLineEdit* mpOrientation_deg = nullptr;
	QComboBox* mpHorizontalAlignment = nullptr;
	QComboBox* mpVerticalAlignment = nullptr;

	QCheckBox* mpColorSameAsBox = nullptr;
	QLineEdit* mpCaptionRed   = nullptr;
	QLineEdit* mpCaptionGreen = nullptr;
	QLineEdit* mpCaptionBlue  = nullptr;
	QLineEdit* mpCaptionAlpha = nullptr;

	QLineEdit* mpNorth_m = nullptr;
	QLineEdit* mpSouth_m = nullptr;
	QLineEdit* mpEast_m  = nullptr;
	QLineEdit* mpWest_m  = nullptr;

	QLineEdit* mpBoxRed   = nullptr;
	QLineEdit* mpBoxGreen = nullptr;
	QLineEdit* mpBoxBlue  = nullptr;
	QLineEdit* mpBoxAlpha = nullptr;
};