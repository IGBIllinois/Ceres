
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
    QString text = device;
    text += ",\t";
    switch (type)
    {
    case 0:
        text += "Status,\t";
        break;
    case 1:
        text += "Info,\t";
        break;
    case 2:
        text += "Warning,\t";
        break;
    case 3:
        text += "Error,\t";
        break;
    }
    text += msg;
    mpLogWindow->appendMessage(text);
}
