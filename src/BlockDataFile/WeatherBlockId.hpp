

#include "BlockId.hpp"

// Forward Declares
namespace weather
{
    enum class DataID : uint16_t;
}

class cWeatherID : public cBlockID
{
public:
    explicit cWeatherID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(weather::DataID id) noexcept;

};


