
#pragma once

//#include "Spidercam/SpidercamScanArea.hpp"
#include "ExperimentFile.hpp"
#include "FieldLayoutWidget.hpp"

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


class cExperimentFieldLayoutDlg : public QDialog
{
public:
	cExperimentFieldLayoutDlg(cFieldLayoutWidget& fieldWidget, QWidget* parent = nullptr);
	virtual ~cExperimentFieldLayoutDlg();

	const cSpidercamScanArea::experimentLayout_t& getLayout() const;

	const cSpidercamScanArea::experimentLayout_t& getOriginalLayout() const;

	void setExperiment(const cExperimentFile& experiment);

private slots:
	void onNewLayout();
	void onLayoutChange(const QString& text);
	void accept() override;

private:
	void createControls();
	void createLayout();

private:
	cFieldLayoutWidget& mFieldWidget;

	cSpidercamScanArea::experimentLayout_t mOriginalLayout;
	cSpidercamScanArea::experimentLayout_t mLayout;

	QComboBox* mpPlotLayouts = nullptr;

	QPushButton* mpNewPlotLayout = nullptr;

	QLineEdit* mpNorthPadding_m = nullptr;
	QLineEdit* mpSouthPadding_m = nullptr;
	QLineEdit* mpEastPadding_m  = nullptr;
	QLineEdit* mpWestPadding_m  = nullptr;

	QString mName;

	float mStartX_m = 0.0;
	float mStartY_m = 0.0;

	float mEndX_m = 0.0;
	float mEndY_m = 0.0;
};