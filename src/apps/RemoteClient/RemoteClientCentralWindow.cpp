
#include "RemoteClientCentralWindow.hpp"
#include "QLogWidget.hpp"

#include <QDebug>


//-----------------------------------------------------------------------------
cRemoteClientCentalWindow::cRemoteClientCentalWindow(QWidget* parent) :
    QTabWidget(parent), mpLogWindow(nullptr)
{
    mpLogWindow = new QLogWidget();

    addTab(mpLogWindow, "Log");
}

//-----------------------------------------------------------------------------
cRemoteClientCentalWindow::~cRemoteClientCentalWindow()
{
}

//-----------------------------------------------------------------------------
void cRemoteClientCentalWindow::updateSensorName(QString old_name, QString new_name)
{
    qInfo() << "Changing sensor name from ";
    qInfo() << old_name;
    qInfo() << " to ";
    qInfo() << new_name;
    qInfo() << "\n";

    auto n = count();
    for (int i = 0; i < n; ++i)
    {
        if (tabText(i) == old_name)
        {
            setTabText(i, new_name);
            break;
        }
    }
}

//-----------------------------------------------------------------------------
void cRemoteClientCentalWindow::logMessage(uint8_t type, QString device, QString msg)
{
    switch (type)
    {
    case 0:
        mpLogWindow->logStatusMessage(device, msg);
        break;
    case 1:
        mpLogWindow->logInfoMessage(device, msg);
        break;
    case 2:
        mpLogWindow->logWarningMessage(device, msg);
        break;
    case 3:
        mpLogWindow->logErrorMessage(device, msg);
        break;
    default:
        mpLogWindow->logMessage(device, msg);
        break;
    }
}
