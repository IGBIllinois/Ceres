
#include "PauseExperimentStateDlg.hpp"

#include <QLayout>
#include <QLabel>
#include <QDialogButtonBox>


cPauseExperimentStateDlg::cPauseExperimentStateDlg()
	: QDialog(), mResult(eRESULT::NONE)
{
	setWindowTitle("Experiment Paused");

	mpContinue = new QPushButton("Continue", this);
	mpContinue->setDefault(true);

	mpAbort = new QPushButton("Abort", this);;

	connect(mpContinue, &QPushButton::pressed, this, &cPauseExperimentStateDlg::accept);
	connect(mpAbort, &QPushButton::pressed, this, &cPauseExperimentStateDlg::reject);

    QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addWidget(new QLabel("Waiting for Continue or Abort button to be selected."));

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
	buttonBox->addButton(mpContinue, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(mpAbort, QDialogButtonBox::RejectRole);

    pMainLayout->addWidget(buttonBox);

    setLayout(pMainLayout);
}

cPauseExperimentStateDlg::~cPauseExperimentStateDlg()
{
}


cPauseExperimentStateDlg::eRESULT cPauseExperimentStateDlg::result()
{
	return mResult;
}

void cPauseExperimentStateDlg::showDlg()
{
	show();
	raise();
	activateWindow();
}


void cPauseExperimentStateDlg::accept()
{
	mResult = eRESULT::CONTINUE;
	QDialog::accept();
}

void cPauseExperimentStateDlg::reject()
{
	mResult = eRESULT::ABORT;
	QDialog::reject();
}

