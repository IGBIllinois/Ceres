
#include "PvtBlockId.hpp"

#include "ClassIdentifiers.hpp"


cPvtID::cPvtID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::OUSTER_LIDAR, majorVer, minorVer)
{}

void cPvtID::dataID(pvt::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


