
#include "ExperimentBlockId.hpp"

#include "ClassIdentifiers.hpp"


cExperimentID::cExperimentID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::EXPERIMENT_INFO, majorVer, minorVer)
{}

void cExperimentID::dataID(experiment::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


