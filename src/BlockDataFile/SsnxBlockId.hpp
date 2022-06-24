

#include "BlockId.hpp"


// Forward Declares
namespace ssnx
{
    enum class DataID : uint16_t;
}

class cSsnxID : public cBlockID
{
public:
    explicit cSsnxID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(ssnx::DataID id) noexcept;
};


