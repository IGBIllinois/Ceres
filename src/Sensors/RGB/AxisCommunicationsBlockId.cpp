
#include "AxisCommunicationsBlockId.hpp"

#include "../../BlockDataFile/ClassIdentifiers.hpp"


cAxisCommunicationsID::cAxisCommunicationsID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::AXIS_COMMUNICATIONS_CAMERA, majorVer, minorVer)
{}

void cAxisCommunicationsID::dataID(axis::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


