
#include "HySpexCommandDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include <algorithm>

namespace
{
	static const QString VNIR_3000N("VNIR-3000N");
	static const QString SWIR_384("SWIR-384");

	static const QString OPEN_SHUTTER("Open Shutter");
	static const QString CLOSE_SHUTTER("Close Shutter");
	static const QString BACKGROUND("Collect Background Image");
}


cHySpexCommandDlg::cHySpexCommandDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("HySpex Command");

	createControls();
	createLayout();
}

cHySpexCommandDlg::~cHySpexCommandDlg()
{}

void cHySpexCommandDlg::createControls()
{
	mpCamera = new QComboBox(this);
	mpCamera->addItem(VNIR_3000N);
	mpCamera->addItem(SWIR_384);

	mpCommand = new QComboBox(this);
	mpCommand->addItem(OPEN_SHUTTER);
	mpCommand->addItem(CLOSE_SHUTTER);
	mpCommand->addItem(BACKGROUND);
}

void cHySpexCommandDlg::createLayout()
{
	QLabel* pText = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pInfo = new QGridLayout();
	pInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("HySpex Camera Type");
	pInfo->addWidget(pText, 0, 0);
	pInfo->addWidget(mpCamera, 0, 1);

	pText = new QLabel("Command to Send");
	pInfo->addWidget(pText, 1, 0);
	pInfo->addWidget(mpCommand, 1, 1);


	pMainLayout->addLayout(pInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

cHySpexCommandDlg::eCameraType cHySpexCommandDlg::getCameraType() const
{
	QString type = mpCamera->currentText();

	if (type == VNIR_3000N)
		return eCameraType::eVNIR_3000N;

	if (type == SWIR_384)
		return eCameraType::eSWIR_384;

	return eCameraType::eUNKNOWN;
}

cHySpexCommandDlg::eCommandType cHySpexCommandDlg::getCommand() const
{
	QString command = mpCommand->currentText();

	if (command == OPEN_SHUTTER)
		return eCommandType::eOPEN_SHUTTER;

	if (command == CLOSE_SHUTTER)
		return eCommandType::eCLOSE_SHUTTER;

	if (command == BACKGROUND)
		return eCommandType::eBACKGROUND;

	return eCommandType::eUNKNOWN;
}



