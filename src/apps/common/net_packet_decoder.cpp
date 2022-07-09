

#include "net_packet_decoder.hpp"
#include "net_buffer.hpp"
#include "packet_utils.hpp"

void cNetworkDecoder::decode(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    ceres::net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

    sPacketHeader_t hdr;

    while (buffer.size() > 0)
    {
        buffer >> hdr;
     
        if ((buf_length < hdr.length) || (hdr.length == 0))
        { 
            break;
        }

        switch (hdr.id)
        {
            case ePacketType::UNKNOWN:
            default:
            {
                break;
            }
            case ePacketType::EXPERIMENT_INFO_1:
            {
                ExperimentInfo_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                sExperimentInfo_t data = to_experiment_info_1(packet);
                experimentInfo(data.title, data.researcher, data.cultivar, data.doc);
                break;
            }
            case ePacketType::SPIDER_CAM_DATA_1:
            {
                Spidercam_Position_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                spidercamPosition(to_spidercam_position_1(packet));
                break;
            }
            case ePacketType::WEATHER_DATA_1:
            {
                WeatherData_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                sWeatherData_t data = to_weather_data_1(packet);
                weatherData(data.dataValid, data.wind_speed_mps, data.wind_direction_deg);
                break;
            }
        }

        buffer.advance(hdr.length);
    }
}


