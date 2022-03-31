
#pragma once

#include "../../BlockDataFile/BlockId.hpp"

// Forward Declares
namespace hyspex
{
    enum class DataID : uint16_t;
}

class cHySpex_VNIR_3000N_ID : public cBlockID
{
public:
    explicit cHySpex_VNIR_3000N_ID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(hyspex::DataID id) noexcept;
};


class cHySpex_SWIR_384_ID : public cBlockID
{
public:
    explicit cHySpex_SWIR_384_ID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(hyspex::DataID id) noexcept;
};


