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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QPlainTextEdit;
class QTextBrowser;
class QPushButton;
QT_END_NAMESPACE


class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void onCreditsOrLicenceChecked(bool checked);

private:
    void createWidgets();
    void createLayouts();

private:
    QLabel* lblLogo = nullptr;
    QLabel* lblProjectTitle = nullptr;

    QWidget* wdtContent = nullptr;
    QLabel* lblVersion = nullptr;
    QTextBrowser* lblDescription = nullptr;
    QLabel* lblDomain = nullptr;
    QLabel* lblCopying = nullptr;

    QTextBrowser* txtBws = nullptr;

    QPushButton* btnCredits = nullptr;
    QPushButton* btnLicense = nullptr;
    QPushButton* btnClose = nullptr;

    static const QString DESCRIPTION;
    static const QString LICENSE;
    static const QString CREDITS;
};

#endif // ABOUTDIALOG_H
