
#include "SpidercamBlockId.hpp"

#include "../BlockDataFile/ClassIdentifiers.hpp"


cSpidercamID::cSpidercamID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::SPIDERCAM, majorVer, minorVer)
{}

void cSpidercamID::dataID(spidercam::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


