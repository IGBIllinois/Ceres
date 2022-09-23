
#include "SensorPropertyPage.hpp"

#include <QAction>
#include <QDialogButtonBox>


cSensorPropertyPage::cSensorPropertyPage(QWidget* parent)
:
    QWidget(parent)
{
    mpShowAction = new QAction(this);

    connect(mpShowAction, &QAction::triggered, this, &cSensorPropertyPage::showPage);


    mpButtons = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);
}

cSensorPropertyPage::~cSensorPropertyPage()
{
}

void cSensorPropertyPage::setTitle(const QString& text)
{
    setWindowTitle(text);
    mpShowAction->setText(text);
}

void cSensorPropertyPage::showPage()
{
    setTitle("again");
}

QAction* cSensorPropertyPage::showAction() const
{
    return mpShowAction;
}

