
#include "HySpexSWIR_384_Parser.hpp"
#include "HySpexDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace hyspex;

cHySpexSWIR_384_Parser::cHySpexSWIR_384_Parser()
:
    cBlockParser()
{}

cBlockID& cHySpexSWIR_384_Parser::blockID()
{
    return mBlockID;
}

void cHySpexSWIR_384_Parser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    switch (static_cast<hyspex::DataID>(data_id))
    {
    }
}

void cHySpexSWIR_384_Parser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}




