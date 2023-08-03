
#pragma once

#include "WeatherDataModel_Http.hpp"

#include <cbdf/WeatherSerializer.hpp>

#include <string>

class cWeatherDataModel_Http_Wind_T_RH_PAR : public cWeatherDataModel_Http
{
    Q_OBJECT

public:
    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    static char* data_type() { return "wind, temp, rh, par"; };
    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    cWeatherDataModel_Http_Wind_T_RH_PAR(QObject* parent = nullptr);
    virtual ~cWeatherDataModel_Http_Wind_T_RH_PAR() = default;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;

    bool windDataValid() const;
    double windSpeed_mps() const;
    double windDirection_deg() const;

    double temperature_C() const;
    double relativeHumidity_pct() const;

    /* Get PAR in umole/s/m^2*/
    double par_umole() const;

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
    void temperatureChanged(double temp_C);
    void relativeHumidityChanged(double RH_pct);
    void parChanged(double par_umole);

protected:
    void processReply(const std::string& reply) override;

private:
    nlohmann::json mReply;
    std::array<double, 6> mData;
    double mMaxWindSpeed_mps;

    bool mDataValid;
    double mWindSpeed_mps;
    double mWindDirection_deg;

    double mOAT_C;
    double mRH_pct;
    double mPAR_umole;

    std::string mConfigInfo;
    cWeatherSerializer mSerializer;
};

inline bool cWeatherDataModel_Http_Wind_T_RH_PAR::windDataValid() const
{
    return mDataValid;
}

inline double cWeatherDataModel_Http_Wind_T_RH_PAR::windSpeed_mps() const
{
    return mWindSpeed_mps;
}

inline double cWeatherDataModel_Http_Wind_T_RH_PAR::windDirection_deg() const
{
    return mWindDirection_deg;
}

inline double cWeatherDataModel_Http_Wind_T_RH_PAR::temperature_C() const
{
    return mOAT_C;
}

inline double cWeatherDataModel_Http_Wind_T_RH_PAR::relativeHumidity_pct() const
{
    return mRH_pct;
}

inline double cWeatherDataModel_Http_Wind_T_RH_PAR::par_umole() const
{
    return mPAR_umole;
}
