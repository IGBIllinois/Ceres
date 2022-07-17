
#pragma once

#include "WeatherDataModel_Http.hpp"
#include "../../BlockDataFile/WeatherSerializer.hpp"

#include <string>

class cWeatherDataModel_Http_Wind : public cWeatherDataModel_Http
{
    Q_OBJECT

public:
    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    static char* data_type() { return "wind"; };
    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    cWeatherDataModel_Http_Wind(QObject* parent = nullptr);
    virtual ~cWeatherDataModel_Http_Wind() = default;

    bool configure(const nlohmann::json& jsonCfg) override;

    bool windDataValid() const;
    double windSpeed_mps() const;
    double windDirection_deg() const;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader() override;

signals:
    void windDataChanged(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);

protected:
    void processReply(const std::string& reply) override;

private:
    nlohmann::json mReply;
    std::array<double, 3> mData;
    double mMaxWindSpeed_mps;

    double mWindSpeed_mps;
    double mWindDirection_deg;
    bool mDataValid;

    std::string mConfigInfo;
    cWeatherSerializer mSerializer;
};

inline bool cWeatherDataModel_Http_Wind::windDataValid() const
{
    return mDataValid;
}

inline double cWeatherDataModel_Http_Wind::windSpeed_mps() const
{
    return mWindSpeed_mps;
}

inline double cWeatherDataModel_Http_Wind::windDirection_deg() const
{
    return mWindDirection_deg;
}

