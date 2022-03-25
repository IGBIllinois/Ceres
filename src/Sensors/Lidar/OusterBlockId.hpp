

#include "../../BlockDataFile/BlockId.hpp"

// Forward Declares
namespace ouster
{
    enum class DataID : uint16_t;
}

class cOusterID : public cBlockID
{
public:
    explicit cOusterID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(ouster::DataID id) noexcept;

};


