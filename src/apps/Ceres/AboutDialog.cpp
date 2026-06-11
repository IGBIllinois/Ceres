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

const QString AboutDialog::CREDITS = QStringLiteral(
	"Brett Feddersen"
);

const QString AboutDialog::LICENSE = QStringLiteral(
	"BSD 3 - Clause License\n"
	"\n"
	"This license applies to all files in the Ceres repository and source "
	"distribution.  This includes Ceres’s source code, the examples, and "
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
	"and/or other materials provided with the distribution.\n"
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
    QDialog(parent),
    mpUI(new Ui::AboutDialog)
{
    mpUI->setupUi(this);

    mpUI->lblVersion->setText(QString::fromStdString(CERES_VERSION));
    mpUI->lblDescription->setText("The Ceres application runs experiment and controls the Ceres Remote Client.");

    QPixmap pixmap(":/ripe.illinois.edu/about.png");
    mpUI->lblLogo->setPixmap(pixmap.scaled(128, 128));

    QFont font = mpUI->lblProjectTitle->font();
    font.setBold(true);
    mpUI->lblProjectTitle->setFont(font);
    mpUI->lblProjectTitle->setText("Ceres");

//    QString domainTxt("<html><head/><body><a href=\"http://%1\">Web page</a></body></html>");
    mpUI->lblDomain->setText("");

    mpUI->lblCopying->setText(QString("Copyright (c) 2022..2026, Carl R. Woese Institute for Genomic Biology,\n"
		"University of Illinois.  All rights reserved.\n"));

    mpUI->txtBws->setVisible(false);
}

AboutDialog::~AboutDialog()
{
    delete mpUI;
    mpUI = nullptr;
}

void AboutDialog::onCreditsOrLicenceChecked(bool checked)
{
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
}
