/*
 * The NetworkDecoder is used to decode network based packets 
 * You will need to override various virtual methods!
 */

#pragma once

#include <spidercam/spidercam_types.hpp>


#include <cstddef>


class cNetworkDecoder
{
public:

    cNetworkDecoder() = default;
    virtual ~cNetworkDecoder() = default;

    /**
     * @brief The decode method convert the byte stream from a TCP receiver port.
     * The decode the various packets and call the various pure virtual 
     * methods.
     */
    void decode(const void* pBuffer, std::size_t buf_length);

private:
    virtual void experimentInfo(const std::string& title, 
        const std::string& researcher, const std::string& cultivar, const std::string& doc)= 0;

    virtual void openDataFile(const std::string& fileName) = 0;
    virtual void closeDataFile() = 0;
    virtual void dataFileState(bool is_open) = 0;

    virtual void spidercamPosition(const spidercam::sPosition& pos) = 0;

    virtual void weatherData(bool valid, double wind_speed_mps, 
        double wind_direction_deg) = 0;
};


