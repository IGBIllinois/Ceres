
#pragma once

#include "../LidarModel.hpp"

//#include <ouster/ouster_defs.h>
//#include <ouster/OusterLidarData.h>

#include <QObject>

const static uint32_t MIN_RANGE_MM = 300;

class cHokuyoModel : public cLidarModel
{
    Q_OBJECT

public:
    cHokuyoModel(QObject* parent = nullptr);
    virtual ~cHokuyoModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    virtual bool configure(const nlohmann::json& jsonCfg) = 0;


signals:
    void updateLidarData();


public slots:


protected:
//    virtual void onNewData(uint16_t frameID, const cOusterLidarData& data) = 0;

protected:
    int mFrameCounter;
};

