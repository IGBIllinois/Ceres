
#pragma once

#include "DataRepair.hpp"

#include <QWidget>
#include <QTimer>
#include <QStringList>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
QT_END_NAMESPACE


// Forward Declarations


class cCentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cCentralWidget(QWidget* parent = nullptr);
    ~cCentralWidget();

    void initialize();
    
signals:
    void statusMessage(QString msg);
    void errorMessage(QString title, QString msg);

private slots:
    void browseSourceFile();
    void repairDataFiles();
    void fileResultsUpdated(bool valid, QString msg);

private:
    QLineEdit*   mpLoadFailedPath = nullptr;
    QPushButton* mpLoadFailedButton = nullptr;
    QLineEdit*   mpRepairPath = nullptr;
    QPushButton* mpRepairButton = nullptr;
    QListWidget* mpRepairResults = nullptr;

    QStringList mFilesToRepair;
    QString     mCurrentDataDirectory;
    QString     mRepairedDataDirectory;
    QString     mCurrentFileName;
};

