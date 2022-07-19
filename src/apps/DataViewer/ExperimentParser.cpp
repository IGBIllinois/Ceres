
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
        onBeginHeader();
        break;
    case DataID::END_OF_HEADER:
        onEndOfHeader();
        break;
    case DataID::BEGIN_FOOTER:
        onBeginFooter();
        break;
    case DataID::END_OF_FOOTER:
        onEndOfFooter();
        break;
    case DataID::BEGIN_SENSOR_LIST:
        break;
    case DataID::END_OF_SENSOR_LIST:
        break;
    case DataID::SENSOR_DATA_BLOCK_INFO:
        break;
    }
}

void cExperimentParser::processTitle(cDataBuffer& buffer)
{
    std::string title;
    buffer >> title;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processTitle.");

    onExperimentTitle(title);
}

void cExperimentParser::processResearcher(cDataBuffer& buffer)
{
    std::string researcher;
    buffer >> researcher;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processResearcher.");

    onExperimentResearcher(researcher);
}

void cExperimentParser::processCultivar(cDataBuffer& buffer)
{
    std::string cultivar;
    buffer >> cultivar;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processCultivar.");

    onExperimentCultivar(cultivar);
}

void cExperimentParser::processExperimentDoc(cDataBuffer& buffer)
{
    std::string experimentDoc;
    buffer >> experimentDoc;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processExperimentDoc.");

    onExperimentDoc(experimentDoc);
}


/*
void cExperimentParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}
*/







