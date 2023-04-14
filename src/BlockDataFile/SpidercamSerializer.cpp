
#include "SpidercamSerializer.hpp"
#include "SpidercamDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <spidercam/spidercam_types.hpp>

#include <cassert>

using namespace spidercam;

cSpidercamSerializer::cSpidercamSerializer()
:
    cBlockSerializer()
{}

cSpidercamSerializer::cSpidercamSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cSpidercamSerializer::blockID()
{
    return mBlockID;
}

void cSpidercamSerializer::write(const spidercam::sPosition_1_t& pos)
{
    assert(mpDataFile);

    setVersion(1, 0);

    mBlockID.dataID(DataID::DOLLY_POSITION);

    mDataBuffer.clear();
    mDataBuffer << pos.X_mm;
    mDataBuffer << pos.Y_mm;
    mDataBuffer << pos.Z_mm;
    mDataBuffer << pos.height_mm;
    mDataBuffer << pos.speed_mmps;
    mDataBuffer << pos.roll_gimbal_deg;
    mDataBuffer << pos.pan_deg;
    mDataBuffer << pos.pan_speed_dps;
    mDataBuffer << pos.tilt_deg;
    mDataBuffer << pos.tilt_speed_dps;
    mDataBuffer << pos.pitch_deg;
    mDataBuffer << pos.roll_deg;
    mDataBuffer << pos.timestamp;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing spidercam::sPosition data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

}

void cSpidercamSerializer::write(const double x_mm, const double y_mm, const double z_mm,
    const double vx_mmps, const double vy_mmps, const double vz_mmps,
    const double yaw_deg, const double pitch_deg, const double roll_deg)
{
    assert(mpDataFile);

    setVersion(1, 0);

    mBlockID.dataID(DataID::EXPERIMENT_START_DOLLY_INFO);

    mDataBuffer.clear();
    mDataBuffer << x_mm;
    mDataBuffer << y_mm;
    mDataBuffer << z_mm;
    mDataBuffer << vx_mmps;
    mDataBuffer << vy_mmps;
    mDataBuffer << vz_mmps;
    mDataBuffer << yaw_deg;
    mDataBuffer << pitch_deg;
    mDataBuffer << roll_deg;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing spidercam target data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

}
