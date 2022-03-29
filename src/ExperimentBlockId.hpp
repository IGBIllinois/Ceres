

#include "./BlockDataFile/BlockId.hpp"

// Forward Declares
namespace experiment
{
    enum class DataID : uint16_t;
}

class cExperimentID : public cBlockID
{
public:
    explicit cExperimentID(uint8_t majorVer = 0, uint8_t minorVer = 0);

    void dataID(experiment::DataID id) noexcept;

};


