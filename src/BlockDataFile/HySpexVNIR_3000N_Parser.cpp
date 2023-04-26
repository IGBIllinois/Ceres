
#include "HySpexVNIR_3000N_Parser.hpp"
#include "HySpexDataIdentifiers.hpp"

#include <cbdf/BlockDataFile.hpp>


#include <cassert>

using namespace hyspex;

cHySpexVNIR_3000N_Parser::cHySpexVNIR_3000N_Parser()
:
    cBlockParser()
{}

cBlockID& cHySpexVNIR_3000N_Parser::blockID()
{
    return mBlockID;
}

void cHySpexVNIR_3000N_Parser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    switch (static_cast<hyspex::DataID>(data_id))
    {
    }
}

void cHySpexVNIR_3000N_Parser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}





