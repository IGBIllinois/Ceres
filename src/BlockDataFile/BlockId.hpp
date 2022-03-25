
#pragma once

#include <cstdint>
#include <utility>

enum class ClassIDs: uint16_t;

/**
	Storage of the data from the Ceres system is based on a block file format.  

	In this file format, each block contains a block type identifier.  The block type is used
	as a signal on how the data in the block should be decoded.

    Block Type Layout
    Each block consists of three or four fields:

    +------------------------------------------------------------+
    |                          Block Type                        |
    +------------+-----------------+-----------------+-----------+
    |  Class ID  |  Major Version  |  Minor Version  |  Data ID  |
    +------------+-----------------+-----------------+-----------+

    Class ID	   : A two-byte unsigned integer representing the class of the data block.
                     Class ID can be based on sensor manufacturer or other criteria.
                     A class ID of zero or 65535 are reserved.

    Major Version  : A single byte unsigned integer defining the data's class major version.

    Major Version  : A single byte unsigned integer defining the data's class minor version.

    Data ID        : A two-byte unsigned integer used to identify the data block.

    For Example:
    The OUSTER LiDAR has a firmware that can be upgraded.  When we upgrade the firmware,
    it is possible that certain data sent by the LiDAR changes format.  We can use
    the major/minor versions to track these changes.

	The class id is two-byte unsigned integer.  The following regions 

    0               : Reserved   
    1     -  1,024  : Range for Main program / Experiment Header data
    1,025 -  2,048  : Range for Experiment Controller data
    2,049 - 65,534  : Range for Sensor data
    65,535          : Reserved
*/


class cBlockID
{
public:
    cBlockID() {}
        
    explicit cBlockID(ClassIDs classId, uint8_t majorVer = 0, uint8_t minorVer = 0)
    : 
        mClassID(static_cast<uint16_t>(classId)), mMajorVersion(majorVer), mMinorVersion(minorVer)
    {}

    void classID(ClassIDs id) noexcept
    {
        mClassID = static_cast<uint16_t>(id);
    }

    uint16_t classID() const noexcept
    {
        return mClassID;
    }

    void setVersion(uint8_t major, uint8_t minor) noexcept
    {
        mMajorVersion = major;
        mMinorVersion = minor;
    }

    void majorVersion(uint8_t ver) noexcept
    {
        mMajorVersion = ver;
    }

    uint8_t majorVersion() const noexcept
    {
        return mMajorVersion;
    }

    void minorVersion(uint8_t ver) noexcept
    {
        mMinorVersion = ver;
    }

    uint8_t minorVersion() const noexcept
    {
        return mMinorVersion;
    }

    void dataID(uint16_t id) noexcept
    {
        mDataID = id;
    }

    uint16_t dataID() const noexcept
    {
        return mDataID;
    }

/*
    friend void swap(BlockID_t& a, BlockID_t& b) noexcept
    {
        using std::swap;
        swap(static_cast<uint32_t&>(a), static_cast<uint32_t&>(b));
    }
*/

private:
    uint16_t mClassID = 0;
    uint8_t mMajorVersion = 0;
    uint8_t mMinorVersion = 0;
    uint16_t mDataID = 0;
};


