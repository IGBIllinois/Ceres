
#include "HySpexBlockId.hpp"

#include <cbdf/extra/SensorClassIdentifiers.hpp>


cHySpex_VNIR_3000N_ID::cHySpex_VNIR_3000N_ID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(static_cast<BLOCK_CLASS_ID_t>(SensorClassIDs::HYSPEX_VNIR_3000N), majorVer, minorVer)
{}

void cHySpex_VNIR_3000N_ID::dataID(hyspex::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


cHySpex_SWIR_384_ID::cHySpex_SWIR_384_ID(uint8_t majorVer, uint8_t minorVer)
    :
    cBlockID(static_cast<BLOCK_CLASS_ID_t>(SensorClassIDs::HYSPEX_SWIR_384), majorVer, minorVer)
{}

void cHySpex_SWIR_384_ID::dataID(hyspex::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


