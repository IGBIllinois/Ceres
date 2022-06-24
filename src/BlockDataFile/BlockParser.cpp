
#include "BlockParser.hpp"
#include "BlockDataFile.hpp"
#include "BlockId.hpp"


cBlockParser::cBlockParser()
{}

void cBlockParser::setVersion(uint8_t major, uint8_t minor)
{
    blockID().setVersion(major, minor);
}




