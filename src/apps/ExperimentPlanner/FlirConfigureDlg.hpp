
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QDialogButtonBox;
QT_END_NAMESPACE


class cFlirConfigureDlg : public QDialog
{
public:
	cFlirConfigureDlg(QWidget* parent = nullptr);
	virtual ~cFlirConfigureDlg();

	int mode() const;
	int lapseInterval_ms() const;
	double frameRate_fps() const;

	void setMode(int mode);
	void setLapseInterval_ms(int interval_ms);
	void setFrameRate_fps(double frame_rate_fps);

protected slots:
	void modeChanged(int mode);

private:
	void createControls();
	void createLayout();

private:
	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

/*
	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSize = nullptr;
*/

	QLabel* mpFrameRateLabel = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;

	QLabel* mpLapseIntervalLabel = nullptr;
	QLineEdit* mpLapseInterval_s = nullptr;

/*
	QLabel* mpThermalRangeLabel = nullptr;
	QLineEdit* mpThermalRange = nullptr;
*/

	QDialogButtonBox* mpButtons;
};