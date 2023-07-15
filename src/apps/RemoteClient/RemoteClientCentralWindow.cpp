
#include "RemoteClientCentralWindow.hpp"
#include "LogWidget.hpp"


//-----------------------------------------------------------------------------
cRemoteClientCentalWindow::cRemoteClientCentalWindow(QWidget* parent) :
    QTabWidget(parent), mpLogWindow(nullptr)
{
    mpLogWindow = new cLogWidget();

    addTab(mpLogWindow, "Log");
}

//-----------------------------------------------------------------------------
cRemoteClientCentalWindow::~cRemoteClientCentalWindow()
{
}

//-----------------------------------------------------------------------------
void cRemoteClientCentalWindow::updateSensorName(QString old_name, QString new_name)
{
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

}
