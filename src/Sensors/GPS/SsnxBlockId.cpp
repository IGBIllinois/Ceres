
#include "SsnxBlockId.hpp"

#include "../../BlockDataFile/ClassIdentifiers.hpp"


cSsnxID::cSsnxID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::SSNX, majorVer, minorVer)
{}

void cSsnxID::dataID(ssnx::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


