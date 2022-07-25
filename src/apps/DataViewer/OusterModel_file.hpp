
#pragma once

#include "OusterParser.hpp"

#include <ouster/ouster_defs.h>

#include <QObject>

class cOusterModel_file : public QObject, public cOusterParser
{
    Q_OBJECT

public:
    cOusterModel_file(QObject* parent = nullptr);
    virtual ~cOusterModel_file() = default;

};

