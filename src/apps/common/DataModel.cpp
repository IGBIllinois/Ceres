
#include "DataModel.hpp"

cDataModel::cDataModel(QObject* parent)
:
    QObject(parent)
{
}

cDataModel::~cDataModel()
{
}

void cDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}

void cDataModel::onErrorUpdate(QString title, QString msg)
{
    emit errorMessage(title, msg);
}


