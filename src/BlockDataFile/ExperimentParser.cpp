
#include "ExperimentParser.hpp"
#include "ExperimentDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace experiment;

cExperimentParser::cExperimentParser()
:
    cBlockParser()
{
    mYear = 0;
    mMonth = 0;
    mDay = 0;

    mDayOfYear = 0;

    mHour = 0;
    mMinute = 0;
    mSeconds = 0;
}

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
        processExperimentDoc(buffer);
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
        processResearcher(buffer);
        break;
    case DataID::CULTIVAR:
        processCultivar(buffer);
        break;
    case DataID::EXPERIMENT_TITLE:
        processTitle(buffer);
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
    case DataID::EXPERIMENT_DATE:
        processDate(buffer);
        break;
    case DataID::EXPERIMENT_DAY_OF_YEAR:
        processDayOfYear(buffer);
        break;
    case DataID::EXPERIMENT_TIME:
        processTime(buffer);
        break;
    }
}

void cExperimentParser::processBeginHeader(cDataBuffer& buffer)
{
    auto n = buffer.read_size();
    mHasBeginHeader = n == 0;
}

void cExperimentParser::processEndOfHeader(cDataBuffer& buffer)
{
    auto n = buffer.read_size();
    mHasEndOfHeader = n == 0;
}

void cExperimentParser::processBeginFooter(cDataBuffer& buffer)
{
    auto n = buffer.read_size();
    mHasBeginFooter = n == 0;
}

void cExperimentParser::processEndOfFooter(cDataBuffer& buffer)
{
    auto n = buffer.read_size();
    mHasEndOfFooter = n == 0;
}

void cExperimentParser::processTitle(cDataBuffer& buffer)
{
    buffer >> mTitle;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processTitle.");
}

void cExperimentParser::processResearcher(cDataBuffer& buffer)
{
    buffer >> mResearcher;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processResearcher.");
}

void cExperimentParser::processCultivar(cDataBuffer& buffer)
{
    buffer >> mCultivar;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processCultivar.");
}

void cExperimentParser::processExperimentDoc(cDataBuffer& buffer)
{
    buffer >> mExperimentDoc;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processExperimentDoc.");
}

void cExperimentParser::processDate(cDataBuffer& buffer)
{
    buffer >> mYear;
    buffer >> mMonth;
    buffer >> mDay;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processDate.");
}

void cExperimentParser::processDayOfYear(cDataBuffer& buffer)
{
    buffer >> mDayOfYear;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processDayOfYear.");
}

void cExperimentParser::processTime(cDataBuffer& buffer)
{
    buffer >> mHour;
    buffer >> mMinute;
    buffer >> mSeconds;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processTime.");
}


/*
void cExperimentParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}
*/







