

#include "ceres_net_decoder.hpp"
#include "net_buffer.hpp"
#include "packet_utils.hpp"

//#include "spidercam_data.pb.h"

using namespace ceres;

void cCeresNetDecoder::decode(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

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
            case ePacketType::SPIDER_CAM_DATA_1:
            {
//                ReceiverTime_1 packet;
//                packet.ParseFromArray(buffer.data(), hdr.length);
//                receiverTime(to_receiver_time_1(packet));
                break;
            }
            case ePacketType::WEATHER_DATA_1:
            {
//                CovMatrix_1 packet;
//                packet.ParseFromArray(buffer.data(), hdr.length);
//                posCovGeodetic(to_pos_cov_geodetic_1(packet));
                break;
            }
        }

        buffer.advance(hdr.length);
    }
}


