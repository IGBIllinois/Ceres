
#include "OusterBlockId.hpp"

#include "../../BlockDataFile/ClassIdentifiers.hpp"


cOusterID::cOusterID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::OUSTER, majorVer, minorVer)
{}

void cOusterID::dataID(ouster::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


