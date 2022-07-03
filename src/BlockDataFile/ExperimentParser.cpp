
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
    switch (static_cast<experiment::DataID>(data_id))
    {
    }
}

void cExperimentParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}







