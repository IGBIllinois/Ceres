
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
    buffer >> mPosition.X_mm;
    buffer >> mPosition.Y_mm;
    buffer >> mPosition.Z_mm;
    buffer >> mPosition.height_mm;
    buffer >> mPosition.speed_mmps;
    buffer >> mPosition.roll_gimbal_deg;
    buffer >> mPosition.pan_deg;
    buffer >> mPosition.pan_speed_dps;
    buffer >> mPosition.tilt_deg;
    buffer >> mPosition.tilt_speed_dps;
    buffer >> mPosition.pitch_deg;
    buffer >> mPosition.roll_deg;
    buffer >> mPosition.timestamp;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPosition.");
}



