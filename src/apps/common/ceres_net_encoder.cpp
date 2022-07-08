

#include "ceres_net_encoder.hpp"
//#include "ceres_remote_client.pb.h"
#include "packet_utils.hpp"

#include <string>

#if defined(_WIN32)

#include <WinSock2.h>
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    namespace sc = std::chrono;
    sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
    sc::seconds s = sc::duration_cast<sc::seconds>(d);
    tp->tv_sec = s.count();
    tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

    return 0;
}

#else
#include <sys/time.h>
#endif // _WIN32

using namespace ceres;

namespace
{
    void set_timestamp(sPacketHeader_t::sTimestamp* timestamp)
    {
        struct timeval tv;

        gettimeofday(&tv, nullptr);

        timestamp->seconds = tv.tv_sec;
        timestamp->nanos = (tv.tv_usec * 1000);
    }
}

/*
int ssnx::encodeGeodetic(const gps::PVT_Geodetic_2_t& pvt, net_buffer& buffer)
{
    GeodeticPVT_2 packet;

    auto* rcv = packet.mutable_rcv_info();
    rcv->set_datavalid(pvt.dataValid);
    rcv->set_gps_timestamp_s(pvt.timestamp_s);

    packet.set_solutiontype(to_type(pvt.Mode));
    packet.set_heightcomputed(pvt.HeightComputed);
    packet.set_error(pvt.Error);
    packet.set_lat_rad(pvt.Lat_rad);
    packet.set_lon_rad(pvt.Lon_rad);
    packet.set_height_m(pvt.Height_m);
    packet.set_undulation_m(pvt.Undulation_m);
    packet.set_vn_mps(pvt.Vn_mps);
    packet.set_ve_mps(pvt.Ve_mps);
    packet.set_vu_mps(pvt.Vu_mps);
    packet.set_groundtrack_deg(pvt.GroundTrack_deg);
    packet.set_rxclkbias_ms(pvt.RxClkBias_ms);
    packet.set_rxclkdrift_ppm(pvt.RxClkDrift_ppm);
    packet.set_timesystem(to_time_system(pvt.TimeSystem));
    packet.set_datum(to_datum(pvt.Datum));
    packet.set_nrsv(pvt.NrSV);
    packet.set_satclockcorrectionused(pvt.SatClockCorrectionUsed);
    packet.set_rangecorrectionused(pvt.RangeCorrectionUsed);
    packet.set_ionosphericinfoused(pvt.IonosphericInfoUsed);
    packet.set_orbitaccuracyinfoused(pvt.OrbitAccuracyInfoUsed);
    packet.set_precisionapproachmodeactive(pvt.PrecisionApproachModeActive);
    packet.set_referenceid(pvt.ReferenceId);
    packet.set_meancorrage_s(pvt.MeanCorrAge_s);
    packet.set_signalinfo(pvt.SignalInfo);
    packet.set_alertflag(pvt.AlertFlag);

    if (pvt.AgeOfSeed_s.has_value())
        packet.set_ageoflastseed_s(pvt.AgeOfSeed_s.value());

    if (pvt.LastSeed.has_value())
        packet.set_lastseedtype(to_type(pvt.LastSeed.value()));

    if (pvt.Latency_s.has_value())
        packet.set_latency_s(pvt.Latency_s.value());

    if (pvt.HAccuracy_m.has_value())
        packet.set_horzaccuracy_m(pvt.HAccuracy_m.value());

    if (pvt.VAccuracy_m.has_value())
        packet.set_vertaccuracy_m(pvt.VAccuracy_m.value());

    std::string str;
    packet.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = ePacketType::GEODETIC_PVT_2;
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}
*/



cCeresNetEncoder::cCeresNetEncoder(std::size_t capacity)
    : mBuffer(capacity)
{}

/*
void cCeresNetEncoder::sendGeodetic(const gps::PVT_Geodetic_2_t& pvt)
{
    encodeGeodetic(pvt);
    sendOutgoingData(static_cast<const char*>(mBuffer.data()), mBuffer.size());
    clear();
}
*/

