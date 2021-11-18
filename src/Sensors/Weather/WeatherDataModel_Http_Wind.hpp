
#pragma once

#include "WeatherDataModel_Http.hpp"



class cWeatherDataModel_Http_Wind : public cWeatherDataModel_Http
{
public:
    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    static char* data_type() { return "wind"; };

    cWeatherDataModel_Http_Wind();
    virtual ~cWeatherDataModel_Http_Wind() = default;

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader(cBlockDataFile& file) override;

};

