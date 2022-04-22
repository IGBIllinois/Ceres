

#include "../../BlockDataFile/BlockId.hpp"

// Forward Declares
namespace axis
{
    enum class DataID : uint16_t;
}

class cAxisCommunicationsID : public cBlockID
{
public:
    explicit cAxisCommunicationsID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(axis::DataID id) noexcept;

};


