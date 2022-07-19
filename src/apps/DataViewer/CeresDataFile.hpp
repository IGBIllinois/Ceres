/**
 * @file
 */
#pragma once

#include "BlockDataFile.hpp"

#include "ExperimentParser.hpp"


#include <string>

class cCeresDataFile : protected cExperimentParser
{
    
public:
    cCeresDataFile();
    ~cCeresDataFile();

    bool open(const std::string& file_name);
    void close();


    bool hasConfigParam() const { return mHasConfigParameters; }
    bool hasSensorInfo() const { return mHasSensorInfo; }
    bool hasTimestamp() const { return mHasTimeStamp; }
    bool hasSyncPulseIn() const { return mHasSyncPulseIn; }
    bool hasSyncPulseOut() const { return mHasSyncPulseOut; }
    bool hasMultipurposeIo() const { return mHasMultipurposeIo; }
    bool hasNmea() const { return mHasNmea; }
    bool hasTimeInfo() const { return mHasTimeInfo; }
    bool hasBeamIntrinsics() const { return mHasBeamIntrinsics; }
    bool hasImuIntrinsics() const { return mHasImuIntrinsics; }
    bool hasLidarIntrinsics() const { return mHasLidarIntrinsics; }
    bool hasLidarDataFormat() const { return mHasLidarDataFormat; }

    int imuFrameCount() const { return mImuFrameCounter; }
    int lidarFrameCount() const { return mLidarFrameCounter; }


    bool updateData();


private:
/*
    void processConfigParam_2(cDataBuffer& buffer) override;
    void processSensorInfo_2(cDataBuffer& buffer) override;
    void processTimestamp_2(cDataBuffer& buffer) override;
    void processSyncPulseIn_2(cDataBuffer& buffer) override;
    void processSyncPulseOut_2(cDataBuffer& buffer) override;
    void processMultipurposeIO_2(cDataBuffer& buffer) override;
    void processNmea_2(cDataBuffer& buffer) override;
    void processTimeInfo_2(cDataBuffer& buffer) override;
    void processBeamIntrinsics_2(cDataBuffer& buffer) override;
    void processImuIntrinsics_2(cDataBuffer& buffer) override;
    void processLidarIntrinsics_2(cDataBuffer& buffer) override;
    void processLidarDataFormat_2(cDataBuffer& buffer) override;
    void processImuData(cDataBuffer& buffer) override;
    void processLidarData(cDataBuffer& buffer) override;
    void processLidarDataFrameTimestamp(cDataBuffer& buffer) override;
*/

private:

    int mImuFrameCounter;
    int mLidarFrameCounter;

    bool mHasConfigParameters;
    bool mHasSensorInfo;
    bool mHasTimeStamp;
    bool mHasSyncPulseIn;
    bool mHasSyncPulseOut;
    bool mHasMultipurposeIo;
    bool mHasNmea;
    bool mHasTimeInfo;
    bool mHasBeamIntrinsics;
    bool mHasImuIntrinsics;
    bool mHasLidarIntrinsics;
    bool mHasLidarDataFormat;
    bool mHasFrameTimestamp;

    cBlockDataFileReader mFileReader;
};

