
#include "SpidercamParser.hpp"
#include "SpidercamDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace spidercam;

cSpidercamParser::cSpidercamParser()
:
    cBlockParser()
{}

cBlockID& cSpidercamParser::blockID()
{
    return mBlockID;
}

void cSpidercamParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    mBlockID.setVersion(major_version, minor_version);
    mBlockID.dataID(static_cast<spidercam::DataID>(data_id));

    switch (static_cast<spidercam::DataID>(data_id))
    {
    case DataID::DOLLY_POSITION:
        processPosition(buffer);
        break;
    }
}

void cSpidercamParser::processPosition(cDataBuffer& buffer)
{
    sPosition_1_t pos;

    buffer >> pos.X_mm;
    buffer >> pos.Y_mm;
    buffer >> pos.Z_mm;
    buffer >> pos.height_mm;
    buffer >> pos.speed_mmps;
    buffer >> pos.roll_gimbal_deg;
    buffer >> pos.pan_deg;
    buffer >> pos.pan_speed_dps;
    buffer >> pos.tilt_deg;
    buffer >> pos.tilt_speed_dps;
    buffer >> pos.pitch_deg;
    buffer >> pos.roll_deg;
    buffer >> pos.timestamp;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPosition.");

    onPosition(pos);
}



