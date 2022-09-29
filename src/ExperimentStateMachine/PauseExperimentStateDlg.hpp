
#pragma once

#include <QDialog>
#include <QPushButton>

class cPauseExperimentStateDlg : public QDialog
{
public:

	enum class eRESULT { NONE, CONTINUE, ABORT };

public:
	cPauseExperimentStateDlg();
	~cPauseExperimentStateDlg();

	eRESULT result();

public slots:
	void showDlg();

protected:
	void accept() override;
	void reject() override;

private:
	QPushButton* mpContinue;
	QPushButton* mpAbort;

	eRESULT mResult;
};


