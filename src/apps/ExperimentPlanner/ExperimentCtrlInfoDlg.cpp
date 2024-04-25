
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentCtrlInfo.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>

namespace
{
	constexpr int DUMMY = 0;
	constexpr int SPIDERCAM = DUMMY + 1;

	const QString CONTROLLER_DUMMY = "Dummy";
	const QString CONTROLLER_SPIDERCAM = "SpiderCam";
}


cExperimentCtrlInfoDlg::cExperimentCtrlInfoDlg(cExperimentFile& info, QWidget* parent)
:
	mInfo(info), QDialog(parent)
{
	setWindowTitle("Experiment Controller");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cExperimentCtrlInfoDlg::~cExperimentCtrlInfoDlg()
{}

void cExperimentCtrlInfoDlg::createControls()
{
	QFont font;
	QFontMetrics fm(font);
	int pixelsHigh = fm.height();

	mpController = new QComboBox(this);
	mpController->addItem(CONTROLLER_DUMMY);
	mpController->addItem(CONTROLLER_SPIDERCAM);

	mpController->setEditable(false);
	connect(mpController, &QComboBox::currentTextChanged, this, &cExperimentCtrlInfoDlg::onControllerChange);

	mpControllerPanels = new QStackedLayout();

	mpCP_Dummy = new QWidget(this);

	mpCP_SpiderCam = new QWidget(this);

	mpSC_UpdateInterval_ms = new QLineEdit(this);
	mpSC_UpdateInterval_ms->setValidator(new QIntValidator(200, 1000));
	mpSC_UpdateInterval_ms->setText("250");

	mpSC_PositionTolerance_cm = new QLineEdit(this);
	mpSC_PositionTolerance_cm->setValidator(new QDoubleValidator(1.0, 100.0, 1));
	mpSC_PositionTolerance_cm->setText("1.0");

	mpActiveController = mInfo.getController();

	if (mpActiveController)
	{
		std::string name = mpActiveController->getType();

		if (name == cExperimentCtrlInfo_Dummy::type())
		{
			mpController->setCurrentIndex(0);
		}

		if (name == cExperimentCtrlInfo_SpiderCam::type())
		{
			mpController->setCurrentIndex(1);

			auto* controller = static_cast<cExperimentCtrlInfo_SpiderCam*>(mpActiveController);
			mpSC_UpdateInterval_ms->setText(QString::number(controller->getUpdateInterval_ms()));
			mpSC_PositionTolerance_cm->setText(QString::number(controller->getPositionTolerance_cm()));
		}
	}
}

void cExperimentCtrlInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Controller Type");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpController);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);

	/** Dummy Panel **/
	QVBoxLayout* pDummyInfo = new QVBoxLayout();

	pText = new QLabel("No Parameters");
	pDummyInfo->addWidget(pText, 0);

	mpCP_Dummy->setLayout(pDummyInfo);

	mpControllerPanels->insertWidget(DUMMY, mpCP_Dummy);

	/** SpiderCam Panel **/
	QGridLayout* pScInfo = new QGridLayout();

	pText = new QLabel("Update Interval (ms)");
	pScInfo->addWidget(pText, 0, 0);
	pScInfo->addWidget(mpSC_UpdateInterval_ms, 0, 1);

	pText = new QLabel("Position Tolerance (cm)");
	pScInfo->addWidget(pText, 1, 0);
	pScInfo->addWidget(mpSC_PositionTolerance_cm, 1, 1);

	mpCP_SpiderCam->setLayout(pScInfo);

	mpControllerPanels->insertWidget(SPIDERCAM, mpCP_SpiderCam);

	pMainLayout->addLayout(mpControllerPanels);

	pMainLayout->addSpacing(10);


	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);

	if (mpActiveController)
	{
		std::string name = mpActiveController->getType();

		if (name == cExperimentCtrlInfo_Dummy::type())
		{
			mpControllerPanels->setCurrentIndex(DUMMY);
		}

		if (name == cExperimentCtrlInfo_SpiderCam::type())
		{
			mpControllerPanels->setCurrentIndex(SPIDERCAM);
		}
	}
}

void cExperimentCtrlInfoDlg::accept()
{
	auto index = mpController->currentIndex();

	if (mpActiveController && (index == 1) && (mpActiveController->getType() == cExperimentCtrlInfo_SpiderCam::type()))
	{
		auto* spidercam = static_cast<cExperimentCtrlInfo_SpiderCam*>(mpActiveController);

		int interval = mpSC_UpdateInterval_ms->text().toInt();
		spidercam->setUpdateInterval_ms(interval);

		float tolerance = mpSC_PositionTolerance_cm->text().toFloat();
		spidercam->setPositionTolerance_cm(tolerance);
	}
	else
	{
		std::unique_ptr<cExperimentCtrlInfo> controller;

		switch (index)
		{
		case 0:
			controller = std::make_unique<cExperimentCtrlInfo_Dummy>();
			break;
		case 1:
			std::unique_ptr<cExperimentCtrlInfo_SpiderCam> spidercam = std::make_unique<cExperimentCtrlInfo_SpiderCam>();

			int interval = mpSC_UpdateInterval_ms->text().toInt();
			spidercam->setUpdateInterval_ms(interval);

			float tolerance = mpSC_PositionTolerance_cm->text().toFloat();
			spidercam->setPositionTolerance_cm(tolerance);

			controller = std::move(spidercam);
			break;
		}

		mInfo.setController(std::move(controller));
	}

	QDialog::accept();
}

void cExperimentCtrlInfoDlg::onControllerChange(const QString& text)
{
	if (text == CONTROLLER_DUMMY)
	{
		mpControllerPanels->setCurrentIndex(DUMMY);
		return;
	}

	if (text == CONTROLLER_SPIDERCAM)
	{
		mpControllerPanels->setCurrentIndex(SPIDERCAM);
		return;
	}
}
