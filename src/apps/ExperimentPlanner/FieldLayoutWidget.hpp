
#pragma once

#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE

// Forward Declarations
class cSpidercamScanArea;


class cFieldLayoutWidget : public QWidget
{
	Q_OBJECT

public:
	cFieldLayoutWidget(QWidget* parent = nullptr);

	void initialize();

protected:
//	void paintEvent(QPaintEvent* event) override;

private:
	cSpidercamScanArea* mpScanArea = nullptr;

};
