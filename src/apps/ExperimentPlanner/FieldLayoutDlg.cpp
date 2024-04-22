
#include "FieldLayoutDlg.hpp"

#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>


cFieldLayoutDlg::cFieldLayoutDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Field Layout");

	createControls();
	createLayout();
}

cFieldLayoutDlg::~cFieldLayoutDlg()
{}


const cSpidercamScanArea::experimentLayout_t& cFieldLayoutDlg::getLayout() const
{
	return mLayout;
}

void cFieldLayoutDlg::setDefaults(const cSpidercamScanArea::experimentLayout_t& layout)
{
	mLayout = layout;

	mpNorth_m->setText(QString::number(mLayout.north_m));
	mpSouth_m->setText(QString::number(mLayout.south_m));
	mpEast_m->setText(QString::number(mLayout.east_m));
	mpWest_m->setText(QString::number(mLayout.west_m));

	mpBoxRed->setText(QString::number(mLayout.color.red()));
	mpBoxGreen->setText(QString::number(mLayout.color.green()));
	mpBoxBlue->setText(QString::number(mLayout.color.blue()));
	mpBoxAlpha->setText(QString::number(mLayout.color.alpha()));

	mpCaption->setText(mLayout.caption.label);
	mpFontSize->setText(QString::number(mLayout.caption.font_size));
	mpOrientation_deg->setText(QString::number(mLayout.caption.orientation_deg));

	switch (mLayout.caption.horizontal_align)
	{
	case cSpidercamScanArea::eHorizontalAlignment::LEFT:
		mpHorizontalAlignment->setCurrentIndex(0);
		break;
	case cSpidercamScanArea::eHorizontalAlignment::CENTER:
		mpHorizontalAlignment->setCurrentIndex(1);
		break;
	case cSpidercamScanArea::eHorizontalAlignment::RIGHT:
		mpHorizontalAlignment->setCurrentIndex(2);
		break;
	}

	switch (mLayout.caption.vertical_align)
	{
	case cSpidercamScanArea::eVerticalAlignment::TOP:
		mpVerticalAlignment->setCurrentIndex(0);
		break;
	case cSpidercamScanArea::eVerticalAlignment::CENTER:
		mpVerticalAlignment->setCurrentIndex(1);
		break;
	case cSpidercamScanArea::eVerticalAlignment::BOTTOM:
		mpVerticalAlignment->setCurrentIndex(2);
		break;
	}

	mpCaptionRed->setText(QString::number(mLayout.caption.color.red()));
	mpCaptionGreen->setText(QString::number(mLayout.caption.color.green()));
	mpCaptionBlue->setText(QString::number(mLayout.caption.color.blue()));
	mpCaptionAlpha->setText(QString::number(mLayout.caption.color.alpha()));

	mpColorSameAsBox->setChecked(mLayout.color == mLayout.caption.color);
}

void cFieldLayoutDlg::accept()
{
	mLayout.north_m = mpNorth_m->text().toDouble();
	mLayout.south_m = mpSouth_m->text().toDouble();
	mLayout.west_m = mpWest_m->text().toDouble();
	mLayout.east_m = mpEast_m->text().toDouble();

	QColor color;

	color.setRed(mpBoxRed->text().toInt());
	color.setGreen(mpBoxGreen->text().toInt());
	color.setBlue(mpBoxBlue->text().toInt());
	color.setAlpha(mpBoxAlpha->text().toInt());

	mLayout.color = color;

	mLayout.caption.label = mpCaption->text();
	mLayout.caption.font_size = static_cast<uint8_t>(mpFontSize->text().toInt());
	mLayout.caption.orientation_deg = mpOrientation_deg->text().toFloat();

	if (!mpColorSameAsBox->isChecked())
	{
		color.setRed(mpCaptionRed->text().toInt());
		color.setGreen(mpCaptionGreen->text().toInt());
		color.setBlue(mpCaptionBlue->text().toInt());
		color.setAlpha(mpCaptionAlpha->text().toInt());
	}

	mLayout.caption.color = color;

	switch (mpHorizontalAlignment->currentIndex())
	{
	case 0:
		mLayout.caption.horizontal_align = cSpidercamScanArea::eHorizontalAlignment::LEFT;
		break;
	case 1:
		mLayout.caption.horizontal_align = cSpidercamScanArea::eHorizontalAlignment::CENTER;
		break;
	case 2:
		mLayout.caption.horizontal_align = cSpidercamScanArea::eHorizontalAlignment::RIGHT;
		break;
	default:
		break;
	}

	switch (mpVerticalAlignment->currentIndex())
	{
	case 0:
		mLayout.caption.vertical_align = cSpidercamScanArea::eVerticalAlignment::TOP;
		break;
	case 1:
		mLayout.caption.vertical_align = cSpidercamScanArea::eVerticalAlignment::CENTER;
		break;
	case 2:
		mLayout.caption.vertical_align = cSpidercamScanArea::eVerticalAlignment::BOTTOM;
		break;
	default:
		break;
	}

	mLayout.computeBounds();

	QDialog::accept();
}

void cFieldLayoutDlg::createControls()
{
	mpCaption = new QLineEdit(this);
	mpFontSize = new QLineEdit(this);
	mpFontSize->setValidator(new QIntValidator(8, 24));

	mpOrientation_deg = new QLineEdit(this);
	mpOrientation_deg->setValidator(new QDoubleValidator(0.0, 360.0, 1));

	mpHorizontalAlignment = new QComboBox(this);
	mpHorizontalAlignment->addItem("LEFT");
	mpHorizontalAlignment->addItem("CENTER");
	mpHorizontalAlignment->addItem("RIGHT");

	mpVerticalAlignment = new QComboBox(this);
	mpVerticalAlignment->addItem("TOP");
	mpVerticalAlignment->addItem("CENTER");
	mpVerticalAlignment->addItem("BOTTOM");

	mpColorSameAsBox = new QCheckBox("Caption Color Same As Bounding Box", this);

	mpCaptionRed = new QLineEdit(this);
	mpCaptionRed->setValidator(new QIntValidator(0, 255));
	connect(mpColorSameAsBox, &QCheckBox::stateChanged, mpCaptionRed, &QLineEdit::setDisabled);

	mpCaptionGreen = new QLineEdit(this);
	mpCaptionGreen->setValidator(new QIntValidator(0, 255));
	connect(mpColorSameAsBox, &QCheckBox::stateChanged, mpCaptionGreen, &QLineEdit::setDisabled);

	mpCaptionBlue = new QLineEdit(this);
	mpCaptionBlue->setValidator(new QIntValidator(0, 255));
	connect(mpColorSameAsBox, &QCheckBox::stateChanged, mpCaptionBlue, &QLineEdit::setDisabled);

	mpCaptionAlpha = new QLineEdit(this);
	mpCaptionAlpha->setValidator(new QIntValidator(0, 255));
	connect(mpColorSameAsBox, &QCheckBox::stateChanged, mpCaptionAlpha, &QLineEdit::setDisabled);

	mpNorth_m = new QLineEdit(this);
	mpNorth_m->setValidator(new QDoubleValidator(0.0, 190.0, 3));

	mpSouth_m = new QLineEdit(this);
	mpSouth_m->setValidator(new QDoubleValidator(0.0, 190.0, 3));

	mpEast_m = new QLineEdit(this);
	mpEast_m->setValidator(new QDoubleValidator(0.0, 190.0, 3));

	mpWest_m = new QLineEdit(this);
	mpWest_m->setValidator(new QDoubleValidator(0.0, 190.0, 3));

	mpBoxRed = new QLineEdit(this);
	mpBoxRed->setValidator(new QIntValidator(0, 255));

	mpBoxGreen = new QLineEdit(this);
	mpBoxGreen->setValidator(new QIntValidator(0, 255));

	mpBoxBlue = new QLineEdit(this);
	mpBoxBlue->setValidator(new QIntValidator(0, 255));

	mpBoxAlpha = new QLineEdit(this);
	mpBoxAlpha->setValidator(new QIntValidator(0, 255));
}

void cFieldLayoutDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGroupBox = new QGroupBox(tr("Bounding Box Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("North (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpNorth_m, 0, 1);

	pText = new QLabel("South (m)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpSouth_m, 0, 4);

	pText = new QLabel("West (m)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpWest_m, 0, 7);

	pText = new QLabel("East (m)");
	pGridLayout->addWidget(pText, 0, 9);
	pGridLayout->addWidget(mpEast_m, 0, 10);


	pText = new QLabel("Red");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpBoxRed, 2, 1);

	pText = new QLabel("Green");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpBoxGreen, 2, 4);

	pText = new QLabel("Blue");
	pGridLayout->addWidget(pText, 2, 6);
	pGridLayout->addWidget(mpBoxBlue, 2, 7);

	pText = new QLabel("Alpha");
	pGridLayout->addWidget(pText, 2, 9);
	pGridLayout->addWidget(mpBoxAlpha, 2, 10);

	pGroupBox->setLayout(pGridLayout);

	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pGroupBox = new QGroupBox(tr("Caption Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();

	QHBoxLayout* pLayout = new QHBoxLayout();

	pText = new QLabel("Caption");
	pLayout->addWidget(pText);
	pLayout->addWidget(mpCaption, 1);

	pVSubLayout->addLayout(pLayout);


	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Font Size");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpFontSize, 0, 1);

	pText = new QLabel("Text Orientation (deg)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpOrientation_deg, 0, 4);

	pText = new QLabel("Horizontal Alignment");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpHorizontalAlignment, 0, 7);

	pText = new QLabel("Vertical Alignment");
	pGridLayout->addWidget(pText, 0, 9);
	pGridLayout->addWidget(mpVerticalAlignment, 0, 10);


	pText = new QLabel("Red");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpCaptionRed, 2, 1);

	pText = new QLabel("Green");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpCaptionGreen, 2, 4);

	pText = new QLabel("Blue");
	pGridLayout->addWidget(pText, 2, 6);
	pGridLayout->addWidget(mpCaptionBlue, 2, 7);

	pText = new QLabel("Alpha");
	pGridLayout->addWidget(pText, 2, 9);
	pGridLayout->addWidget(mpCaptionAlpha, 2, 10);

	pVSubLayout->addLayout(pGridLayout);

	pVSubLayout->addWidget(mpColorSameAsBox);

	pGroupBox->setLayout(pVSubLayout);

	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}



