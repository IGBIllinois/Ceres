
#include "ExperimentParser.hpp"
#include "ExperimentDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace experiment;

cExperimentParser::cExperimentParser()
:
    cBlockParser()
{}

cBlockID& cExperimentParser::blockID()
{
    return mBlockID;
}

void cExperimentParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    mBlockID.setVersion(major_version, minor_version);
    mBlockID.dataID(static_cast<experiment::DataID>(data_id));

    switch (static_cast<experiment::DataID>(data_id))
    {
    case DataID::EXPERIMENT:
        break;
    case DataID::START_TIME:
        break;
    case DataID::END_TIME:
        break;
    case DataID::START_RECORDING_TIMESTAMP:
        break;
    case DataID::END_RECORDING_TIMESTAMP:
        break;
    case DataID::RESEARCHER:
        break;
    case DataID::CULTIVAR:
        break;
    case DataID::EXPERIMENT_TITLE:
        break;
    case DataID::BEGIN_HEADER:
        processBeginHeader(buffer);
        break;
    case DataID::END_OF_HEADER:
        processEndOfHeader(buffer);
        break;
    case DataID::BEGIN_FOOTER:
        processBeginFooter(buffer);
        break;
    case DataID::END_OF_FOOTER:
        processEndOfFooter(buffer);
        break;
    case DataID::BEGIN_SENSOR_LIST:
        break;
    case DataID::END_OF_SENSOR_LIST:
        break;
    case DataID::SENSOR_DATA_BLOCK_INFO:
        break;
    }
}

void cExperimentParser::processBeginHeader(cDataBuffer& buffer)
{
    auto n = buffer.read_size();
}

void cExperimentParser::processEndOfHeader(cDataBuffer& buffer)
{

}

void cExperimentParser::processBeginFooter(cDataBuffer& buffer)
{

}

void cExperimentParser::processEndOfFooter(cDataBuffer& buffer)
{

}

/*
void cExperimentParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}
*/







