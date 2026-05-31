
#pragma once

#include <QDialog>

#include <string>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;
QT_END_NAMESPACE


class cMarkerInfoDlg : public QDialog
{
public:
	enum eType { CUSTOM = 0, START_OF_MEASUREMENT = 1, END_OF_MEASUREMENT = 2 };

public:
	cMarkerInfoDlg(QWidget* parent = nullptr);
	virtual ~cMarkerInfoDlg();

	eType type() const;

	bool hasLabel() const;
	std::string  label() const;

	void setType(eType type);
	void setLabel(const std::string& label);

protected slots:
	void onTypeChanged(int index);

private:
	void createControls();
	void createLayout();

private:
	QComboBox* mpType;

	QLineEdit* mpLabel;
};