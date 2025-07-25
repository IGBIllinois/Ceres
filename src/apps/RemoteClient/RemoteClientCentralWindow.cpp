
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
void cRemoteClientCentalWindow::updateSensorName(QString old_name, QString new_name, QString instance)
{
    QString oname = old_name;

    if (!instance.isEmpty())
    {
        oname += ":";
        oname += instance;
    }

    QString nname = new_name;

    if (!instance.isEmpty())
    {
        nname += ":";
        nname += instance;
    }

    qInfo() << "Changing sensor name from ";
    qInfo() << oname;
    qInfo() << " to ";
    qInfo() << nname;
    qInfo() << "\n";

    auto n = count();
    for (int i = 0; i < n; ++i)
    {
        if (tabText(i) == old_name)
        {
            setTabText(i, nname);
            break;
        }

        if (tabText(i) == oname)
        {
            setTabText(i, nname);
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

//-----------------------------------------------------------------------------
void cRemoteClientCentalWindow::logMessage(uint8_t type, QString device, QString instance, QString msg)
{
    if (instance.isEmpty())
    {
        logMessage(type, device, msg);
        return;
    }

    switch (type)
    {
    case 0:
        mpLogWindow->logStatusMessage(device + ":" + instance, msg);
        break;
    case 1:
        mpLogWindow->logInfoMessage(device + ":" + instance, msg);
        break;
    case 2:
        mpLogWindow->logWarningMessage(device + ":" + instance, msg);
        break;
    case 3:
        mpLogWindow->logErrorMessage(device + ":" + instance, msg);
        break;
    default:
        mpLogWindow->logMessage(device + ":" + instance, msg);
        break;
    }
}
