
#pragma once

#include <string>
#include <nlohmann/json.hpp>

class cExperimentState;


class cExperimentStateCreator
{
public:
    virtual cExperimentState* createState(const std::string& type, const nlohmann::json& stateDoc) = 0;
};


