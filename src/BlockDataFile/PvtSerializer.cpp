
#include "PvtSerializer.hpp"
#include "PvtDataIdentifiers.hpp"
#include "BlockDataFile.hpp"


#include <cassert>

using namespace pvt;

cPvtSerializer::cPvtSerializer()
:
    cBlockSerializer()
{
    setVersion(1, 0);
}

cPvtSerializer::cPvtSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
    setVersion(1, 0);
}

cBlockID& cPvtSerializer::blockID()
{
    return mBlockID;
}

void cPvtSerializer::write(ePOSTION_UNITS unit)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::POSITION_UNIT);

    mDataBuffer.clear();
    mDataBuffer << static_cast<uint8_t>(unit);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write (position units) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write(eVELOCITY_UNITS unit)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::VELOCITY_UNIT);

    mDataBuffer.clear();
    mDataBuffer << static_cast<uint8_t>(unit);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write (velocity units) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write(eTIME_UNITS unit)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::TIME_UNIT);

    mDataBuffer.clear();
    mDataBuffer << static_cast<uint8_t>(unit);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write (time units) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_position(double x)
{
    assert(mpDataFile);

	mBlockID.dataID(DataID::POSITION_1D);

    mDataBuffer.clear();
    mDataBuffer << x;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_position (1d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_position(double x, double y)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::POSITION_2D);

    mDataBuffer.clear();
    mDataBuffer << x;
    mDataBuffer << y;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_position (2d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_position(double x, double y, double z)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::POSITION_3D);

    mDataBuffer.clear();
    mDataBuffer << x;
    mDataBuffer << y;
    mDataBuffer << z;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_position (3d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_velocity(double x)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::VELOCITY_1D);

    mDataBuffer.clear();
    mDataBuffer << x;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_velocity (1d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_velocity(double x, double y)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::VELOCITY_2D);

    mDataBuffer.clear();
    mDataBuffer << x;
    mDataBuffer << y;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_velocity (2d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_velocity(double x, double y, double z)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::VELOCITY_3D);

    mDataBuffer.clear();
    mDataBuffer << x;
    mDataBuffer << y;
    mDataBuffer << z;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_velocity (3d) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cPvtSerializer::write_timestamp(std::uint64_t t)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << t;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing write_timestamp data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

