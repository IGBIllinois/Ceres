

#include "BlockId.hpp"

// Forward Declares
namespace spidercam
{
    enum class DataID : uint16_t;
}

class cSpidercamID : public cBlockID
{
public:
    explicit cSpidercamID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(spidercam::DataID id) noexcept;

};


