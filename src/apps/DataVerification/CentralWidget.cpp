

#include "CentralWidget.hpp"

#include "BlockDataFile.hpp"


#include <QtWidgets>
#include <QLayout>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QThreadPool>

#include <memory>


//-----------------------------------------------------------------------------
cCentralWidget::cCentralWidget(QWidget* parent) :
    QWidget(parent)
{
    initialize();

//    mpVerifier = new cDataVerifier();
//    connect(mpVerifier, &cDataVerifier::fileResults, this, &cCentralWidget::fileResultsUpdated);
}

//-----------------------------------------------------------------------------
cCentralWidget::~cCentralWidget()
{
//    mpVerifier->deleteLater();
}

//-----------------------------------------------------------------------------
void cCentralWidget::initialize()
{
    mpLoadSrcFile = new QLineEdit();
    mpLoadSrcFile->setReadOnly(true);
    mpLoadSrcFile->setMinimumWidth(500);

    mpLoadSrcButton = new QPushButton(this);
    mpLoadSrcButton->setText("Browse");
    mpLoadSrcButton->setEnabled(true);
    connect(mpLoadSrcButton, &QPushButton::pressed, this, &cCentralWidget::browseSourceFile);

    mpScanResults = new QListWidget(this);
    mpScanResults->setSelectionMode(QAbstractItemView::NoSelection);

    // Layout the dialog...

    QVBoxLayout* pMainLayout = new QVBoxLayout();
//    pMainLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto* srcLayout = new QHBoxLayout();

    QLabel* pSourceLabel = new QLabel();
    pSourceLabel->setText("Source Directory:");

    srcLayout->addWidget(pSourceLabel);
    srcLayout->addSpacing(24);
    srcLayout->addWidget(mpLoadSrcFile);
    srcLayout->addSpacing(3);
    srcLayout->addWidget(mpLoadSrcButton);

    pMainLayout->addLayout(srcLayout, 1);
    pMainLayout->addWidget(mpScanResults, 1);

    setLayout(pMainLayout);
}

//-----------------------------------------------------------------------------
void cCentralWidget::browseSourceFile()
{
    QFileDialog dlg(this, "Open Directory");
    dlg.setFileMode(QFileDialog::Directory);

    auto result = dlg.exec();
    if (result == QDialog::Rejected)
        return;

    QStringList fileNames = dlg.selectedFiles();
    if (fileNames.size() == 0)
        return;

    QString path = fileNames[0];

    if (path.isEmpty())
        return;

    mpLoadSrcFile->setText(path);
    mpLoadSrcButton->setEnabled(false);

    mpScanResults->clear();

    emit scanForDataFiles(path);
}

void cCentralWidget::scanForDataFiles(const QString& path)
{
    mFilesToTest.clear();

    QDir dir(path);

    auto files = dir.entryInfoList(QDir::Files);
    for (auto file : files)
    {
        QString test_name = file.fileName();
        if (test_name.contains("ceres") || test_name.contains("lidar_data"))
        {
            mFilesToTest.push_back(file.absoluteFilePath());
        }
    }

    if (mFilesToTest.empty())
    {
        mpLoadSrcButton->setEnabled(true);

        emit statusMessage("Could not find any Ceres data files to scan.");

        return;
    }

    mCurrentFileName = mFilesToTest.front();
    mFilesToTest.pop_front();

    cDataVerifier* pVerifier = new cDataVerifier();
    connect(pVerifier, &cDataVerifier::fileResults, this, &cCentralWidget::fileResultsUpdated);

    pVerifier->open(mCurrentFileName.toStdString());

    auto* pItem = new QListWidgetItem();
    QString text = "Scanning ";
    text += mCurrentFileName;
    text += "...";
    pItem->setText(text);
    mpScanResults->insertItem(mpScanResults->count(), pItem);
    mpScanResults->setCurrentItem(pItem);

    QThreadPool::globalInstance()->start(pVerifier);
}

//-----------------------------------------------------------------------------
void cCentralWidget::fileResultsUpdated(bool valid, QString msg)
{
    emit statusMessage(msg);

    auto* pItem = mpScanResults->currentItem();

    if (valid)
    {
        QString text = mCurrentFileName;
        text += "... passed.";
        pItem->setText(text);
    }
    else
    {
        QString text = mCurrentFileName;
        text += "... failed!";
        pItem->setText(text);
    }

    if (mFilesToTest.empty())
    {
        mpLoadSrcButton->setEnabled(true);

        emit statusMessage("Scan Finished.");

        return;
    }

    mCurrentFileName = mFilesToTest.front();
    mFilesToTest.pop_front();

    cDataVerifier* pVerifier = new cDataVerifier();
    connect(pVerifier, &cDataVerifier::fileResults, this, &cCentralWidget::fileResultsUpdated);

    pVerifier->open(mCurrentFileName.toStdString());

    pItem = new QListWidgetItem();
    QString text = "Scanning ";
    text += mCurrentFileName;
    text += "...";
    pItem->setText(text);
    mpScanResults->insertItem(mpScanResults->count(), pItem);
    mpScanResults->setCurrentItem(pItem);

    QThreadPool::globalInstance()->start(pVerifier);
}

