
#pragma once

#include <QtWidgets>
#include <QDialog>
#include <filesystem>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
class QLabel;
QT_END_NAMESPACE

class cBatchExpConfirmDlg : public QDialog
{
	Q_OBJECT

public:
	cBatchExpConfirmDlg(QWidget* parent = nullptr);
	~cBatchExpConfirmDlg();


private slots:
	void accept() override;
	void reject() override;

private:
};
