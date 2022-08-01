

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
}

//-----------------------------------------------------------------------------
cCentralWidget::~cCentralWidget()
{
}

//-----------------------------------------------------------------------------
void cCentralWidget::initialize()
{
    mpLoadFailedPath = new QLineEdit();
    mpLoadFailedPath->setReadOnly(true);
    mpLoadFailedPath->setMinimumWidth(500);

    mpLoadFailedButton = new QPushButton(this);
    mpLoadFailedButton->setText("Browse");
    mpLoadFailedButton->setEnabled(true);
    connect(mpLoadFailedButton, &QPushButton::pressed, this, &cCentralWidget::browseSourceFile);

    mpRepairPath = new QLineEdit();
    mpRepairPath->setReadOnly(true);
    mpRepairPath->setMinimumWidth(500);

    mpRepairButton = new QPushButton(this);
    mpRepairButton->setText("Repair");
    mpRepairButton->setEnabled(false);
    connect(mpRepairButton, &QPushButton::pressed, this, &cCentralWidget::repairDataFiles);


    mpRepairResults = new QListWidget(this);
    mpRepairResults->setSelectionMode(QAbstractItemView::NoSelection);

    // Layout the dialog...

    QVBoxLayout* pMainLayout = new QVBoxLayout();

    auto* srcLayout = new QHBoxLayout();

    QLabel* pSourceLabel = new QLabel();
    pSourceLabel->setText("Source Directory:");

    srcLayout->addWidget(pSourceLabel);
    srcLayout->addSpacing(24);
    srcLayout->addWidget(mpLoadFailedPath);
    srcLayout->addSpacing(3);
    srcLayout->addWidget(mpLoadFailedButton);

    pMainLayout->addLayout(srcLayout, 1);

    auto* dstLayout = new QHBoxLayout();

    QLabel* pRepairLabel = new QLabel();
    pRepairLabel->setText("Repaired Directory:");

    dstLayout->addWidget(pRepairLabel);
    dstLayout->addSpacing(15);
    dstLayout->addWidget(mpRepairPath);

    pMainLayout->addLayout(dstLayout, 1);

    pMainLayout->addWidget(mpRepairButton, 1);
    pMainLayout->addWidget(mpRepairResults, 1);

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

    mCurrentDataDirectory = path;

    mpLoadFailedPath->setText(path);

    QDir dataPath(path);
    dataPath.cdUp();

    mRepairedDataDirectory = dataPath.path();
    mRepairedDataDirectory += "/repaired/";
    mpRepairPath->setText(mRepairedDataDirectory);

    mpRepairButton->setEnabled(true);

    mpRepairResults->clear();
}

void cCentralWidget::repairDataFiles()
{
    mpLoadFailedButton->setEnabled(false);
    mFilesToRepair.clear();

    if (!QDir().mkpath(mRepairedDataDirectory))
    {
        mpLoadFailedButton->setEnabled(true);

        emit statusMessage("Failed to create destination directory for repaired files.");

        return;
    }

    QDir dir(mCurrentDataDirectory);

    auto files = dir.entryInfoList(QDir::Files);
    for (auto file : files)
    {
        QString test_name = file.fileName();
        if (test_name.contains("ceres") || test_name.contains("lidar_data"))
        {
            mFilesToRepair.push_back(file.absoluteFilePath());
        }
    }

    if (mFilesToRepair.empty())
    {
        mpLoadFailedButton->setEnabled(true);

        emit statusMessage("Could not find any Ceres data files to scan.");

        return;
    }

    mCurrentFileName = mFilesToRepair.front();
    mFilesToRepair.pop_front();

    cDataRepair* pRepair = new cDataRepair(mRepairedDataDirectory);
    connect(pRepair, &cDataRepair::fileResults, this, &cCentralWidget::fileResultsUpdated);
    connect(pRepair, &cDataRepair::statusMessage, this, &cCentralWidget::statusMessage);

    pRepair->open(mCurrentFileName.toStdString());

    auto* pItem = new QListWidgetItem();
    QString text = "Scanning ";
    text += mCurrentFileName;
    text += "...";
    pItem->setText(text);
    mpRepairResults->insertItem(mpRepairResults->count(), pItem);
    mpRepairResults->setCurrentItem(pItem);

    QThreadPool::globalInstance()->start(pRepair);
}

//-----------------------------------------------------------------------------
void cCentralWidget::fileResultsUpdated(bool valid, QString msg)
{
    emit statusMessage(msg);

    auto* pItem = mpRepairResults->currentItem();

    if (valid)
    {
        QString text = mCurrentFileName;
        text += "... repaired.";
        pItem->setText(text);
    }
    else
    {
        QString text = mCurrentFileName;
        text += "... failed!";
        pItem->setText(text);
    }

    if (mFilesToRepair.empty())
    {
        mpLoadFailedButton->setEnabled(true);

        emit statusMessage("Scan Finished.");

        return;
    }

    mCurrentFileName = mFilesToRepair.front();
    mFilesToRepair.pop_front();

    cDataRepair* pRepair = new cDataRepair(mRepairedDataDirectory);
    connect(pRepair, &cDataRepair::fileResults, this, &cCentralWidget::fileResultsUpdated);
    connect(pRepair, &cDataRepair::statusMessage, this, &cCentralWidget::statusMessage);

    pRepair->open(mCurrentFileName.toStdString());

    pItem = new QListWidgetItem();
    QString text = "Scanning ";
    text += mCurrentFileName;
    text += "...";
    pItem->setText(text);
    mpRepairResults->insertItem(mpRepairResults->count(), pItem);
    mpRepairResults->setCurrentItem(pItem);

    QThreadPool::globalInstance()->start(pRepair);
}

