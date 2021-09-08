
#pragma once

#include "DataFile.hpp"

#include <QObject>
#include <vector>

// Forward Declarations

class cSensorModel;

class cDataModel : public QObject
{
    Q_OBJECT

public:
    explicit cDataModel();
    ~cDataModel();

    void addSensor(cSensorModel* pSensor);

    void startDataCollection();

public slots:
    void stopDataCollection();

private:
    std::vector<cSensorModel*> mActiveSensors;

    cDataFile   mFile;
};

