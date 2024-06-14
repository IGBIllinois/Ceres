
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;
QT_END_NAMESPACE


class cHySpexCommandDlg : public QDialog
{
public:
	cHySpexCommandDlg(QWidget* parent = nullptr);
	virtual ~cHySpexCommandDlg();

	enum class eCameraType { eUNKNOWN, eVNIR_3000N, eSWIR_384 };

	eCameraType getCameraType() const;

	enum class eCommandType { eUNKNOWN, eOPEN_SHUTTER, eCLOSE_SHUTTER, eBACKGROUND };

	eCommandType getCommand() const;

private:
	void createControls();
	void createLayout();

private:

	QComboBox* mpCamera;
	QComboBox* mpCommand;
};