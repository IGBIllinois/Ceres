
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
    switch (static_cast<spidercam::DataID>(data_id))
    {
    }
}

void cSpidercamParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}


