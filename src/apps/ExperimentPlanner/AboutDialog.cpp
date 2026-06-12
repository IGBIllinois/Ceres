/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Daniel San
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "AboutDialog.hpp"
#include "ui_aboutdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QSizePolicy>
#include <QRgb>

const QString AboutDialog::DESCRIPTION = QStringLiteral(
	"The Ceres Experiment Planner is an application to generate or edit Ceres\n"
	"experiment files in a graphical way."
);

const QString AboutDialog::CREDITS = QStringLiteral(
	"Brett Feddersen"
);

const QString AboutDialog::LICENSE = QStringLiteral(
	"BSD 3 - Clause License\n"
	"\n"
	"This license applies to all files in the Ceres Remote Client repository and source "
	"distribution.  This includes Ceres Remote Client’s source code, the examples, and "
	"tests, as well as the documentation.\n"
	"\n"
	"Copyright(c) 2022..2026, Carl R.Woese Institute for Genomic Biology\n"
	"All rights reserved.\n"
	"\n"
	"Redistribution and use in source and binary forms, with or without "
	"modification, are permitted provided that the following conditions are met :\n"
	"\n"
	"1. Redistributions of source code must retain the above copyright notice, this "
	"list of conditions and the following disclaimer.\n"
	"\n"
	"2. Redistributions in binary form must reproduce the above copyright notice, "
	"this list of conditions and the following disclaimer in the documentation "
	"and /or other materials provided with the distribution.\n"
	"\n"
	"3. Neither the name of the copyright holder nor the names of its "
	"contributors may be used to endorse or promote products derived from "
	"this software without specific prior written permission.\n"
	"\n"
	"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" "
	"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE "
	"IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE "
	"DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE "
	"FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL "
	"DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR "
	"SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER "
	"CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, "
	"OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE "
	"OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
);

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
	setWindowTitle("About");

	setMinimumSize(450, 400);
	setMaximumSize(450, 400);
	setSizeGripEnabled(false);

	setModal(false);

	createWidgets();
	createLayouts();

	lblVersion->setText(QString::fromStdString(CERES_EXPERIMENT_PLANNER_VERSION));
	lblDescription->setText(DESCRIPTION);

	QPixmap pixmap(":/ripe.illinois.edu/about.png");
	lblLogo->setPixmap(pixmap.scaled(128, 128));

	QFont font = lblProjectTitle->font();
	font.setBold(true);
	lblProjectTitle->setFont(font);
	lblProjectTitle->setText("Ceres Experiment Planner");

	lblDomain->setText("");
	lblDomain->setHidden(true);

	lblCopying->setText(QString("Copyright (c) 2026, Carl R. Woese Institute for Genomic Biology,\n"
		"University of Illinois.  All rights reserved.\n"));

	txtBws->setVisible(false);

/*
	mpUI->setupUi(this);

    mpUI->lblVersion->setText(QString::fromStdString(CERES_EXPERIMENT_PLANNER_VERSION));
    mpUI->lblDescription->setText(QString("The Ceres Experiment Planner is an application to generate or edit Ceres\n"
		"experiment files in a graphical way."));

    QPixmap pixmap(":/ripe.illinois.edu/about.png");
    mpUI->lblLogo->setPixmap(pixmap.scaled(128, 128));

    QFont font = mpUI->lblProjectTitle->font();
    font.setBold(true);
    mpUI->lblProjectTitle->setFont(font);
    mpUI->lblProjectTitle->setText("Ceres Experiment Planner");

    mpUI->lblDomain->setText("");

    mpUI->lblCopying->setText(QString("Copyright (c) 2026, Carl R. Woese Institute for Genomic Biology,\n"
		"University of Illinois.  All rights reserved.\n"));

    mpUI->txtBws->setVisible(false);
*/
}

AboutDialog::~AboutDialog()
{
//    delete mpUI;
//    mpUI = nullptr;
}

void AboutDialog::createWidgets()
{
	lblLogo = new QLabel("logo", this);
	lblProjectTitle = new QLabel("project_title", this);

	wdtContent = new QWidget(this);
	lblVersion = new QLabel("version", wdtContent);

	lblDescription = new QTextBrowser(wdtContent);
	lblDescription->setText("description");
	lblDescription->setReadOnly(true);
	lblDescription->setAcceptDrops(false);
	lblDescription->setOpenExternalLinks(true);
	lblDescription->setFrameStyle(QFrame::NoFrame);
	lblDescription->setWordWrapMode(QTextOption::WordWrap);

	// Get the background color from the dialog's palette
	QPalette palette = this->palette();
	QRgb bgColor = palette.color(QPalette::Window).rgb();

	lblDescription->setStyleSheet("background-color: rgb(240,240,240);"); 

	lblDomain = new QLabel("domain", wdtContent);
	lblDomain->setTextFormat(Qt::TextFormat::RichText);
	lblDomain->setOpenExternalLinks(true);
	lblDomain->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse);

	lblCopying = new QLabel("copying", wdtContent);

	txtBws = new QTextBrowser(this);
	txtBws->setTabChangesFocus(true);
	txtBws->setOpenExternalLinks(true);

	btnCredits = new QPushButton("Credits", this);
	btnCredits->setObjectName("btnCredits");
	QSizePolicy sizePolicy1(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(btnCredits->sizePolicy().hasHeightForWidth());
	btnCredits->setSizePolicy(sizePolicy1);
	btnCredits->setCheckable(true);
	btnCredits->setChecked(false);
	btnCredits->setAutoDefault(true);
	btnCredits->setFlat(false);

	btnLicense = new QPushButton("License", this);
	btnLicense->setObjectName("btnLicense");
	QSizePolicy sizePolicy2(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(btnLicense->sizePolicy().hasHeightForWidth());
	btnLicense->setSizePolicy(sizePolicy2);
	btnLicense->setCheckable(true);

	btnClose = new QPushButton("Close", this);
	sizePolicy2.setHeightForWidth(btnClose->sizePolicy().hasHeightForWidth());
	btnClose->setSizePolicy(sizePolicy2);

	connect(btnClose,   &QPushButton::clicked, this, &QDialog::reject);
	connect(btnCredits, &QPushButton::clicked, this, &AboutDialog::onCreditsOrLicenceChecked);
	connect(btnLicense, &QPushButton::clicked, this, &AboutDialog::onCreditsOrLicenceChecked);

}

void AboutDialog::createLayouts()
{
	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(10);
	mainLayout->setContentsMargins(10, 10, 10, 10);

	mainLayout->addWidget(lblLogo, 0, Qt::AlignmentFlag::AlignHCenter);
	mainLayout->addWidget(lblProjectTitle, 0, Qt::AlignmentFlag::AlignHCenter);

	auto* vBoxLytContent = new QVBoxLayout(wdtContent);
	vBoxLytContent->setSpacing(5);
	vBoxLytContent->setContentsMargins(-1, 5, -1, 1);

	auto* verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(10);
	verticalLayout->setContentsMargins(-1, -1, -1, 10);

	verticalLayout->addWidget(lblVersion, 0, Qt::AlignmentFlag::AlignHCenter);
	verticalLayout->addWidget(lblDescription, 1);
	verticalLayout->addWidget(lblDomain, 0, Qt::AlignmentFlag::AlignHCenter);

	auto* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
	verticalLayout->addItem(verticalSpacer);
	vBoxLytContent->addLayout(verticalLayout);

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

	vBoxLytContent->addItem(verticalSpacer);

	vBoxLytContent->addWidget(lblCopying, 0, Qt::AlignmentFlag::AlignHCenter);

	mainLayout->addWidget(wdtContent);

	mainLayout->addWidget(txtBws);

	auto* horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(5);

	horizontalLayout->addWidget(btnCredits);
	horizontalLayout->addWidget(btnLicense);

	auto* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

	horizontalLayout->addItem(horizontalSpacer);
	horizontalLayout->addWidget(btnClose);

	mainLayout->addLayout(horizontalLayout);

	setLayout(mainLayout);
}

void AboutDialog::onCreditsOrLicenceChecked(bool checked)
{
	if (sender()->objectName() == "btnLicense")
	{
		txtBws->setText(LICENSE);
		btnCredits->setChecked(false);
	}
	else if (sender()->objectName() == "btnCredits")
	{
		txtBws->setText(CREDITS);
		btnLicense->setChecked(false);
	}

	txtBws->setVisible(checked);
	wdtContent->setVisible(!checked);
	
/*
    if (sender()->objectName() == "btnLicense") 
    {
        mpUI->txtBws->setText(LICENSE);
        mpUI->btnCredits->setChecked(false);
    } 
    else if (sender()->objectName() == "btnCredits") 
    {
        mpUI->txtBws->setText(CREDITS);
        mpUI->btnLicense->setChecked(false);
    }

    mpUI->txtBws->setVisible(checked);
    mpUI->wdtContent->setVisible(!checked);
*/
}
