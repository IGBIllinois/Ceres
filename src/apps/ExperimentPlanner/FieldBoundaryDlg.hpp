
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
QT_END_NAMESPACE


class cFieldBoundaryDlg : public QDialog
{
public:
	cFieldBoundaryDlg(QWidget* parent = nullptr);
	virtual ~cFieldBoundaryDlg();

	int minX_mm() const;
	int maxX_mm() const;
	int minY_mm() const;
	int maxY_mm() const;

	void setMinX_mm(int limit);
	void setMaxX_mm(int limit);
	void setMinY_mm(int limit);
	void setMaxY_mm(int limit);

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpMinX_mm = nullptr;
	QLineEdit* mpMaxX_mm = nullptr;
	QLineEdit* mpMinY_mm = nullptr;
	QLineEdit* mpMaxY_mm = nullptr;
};