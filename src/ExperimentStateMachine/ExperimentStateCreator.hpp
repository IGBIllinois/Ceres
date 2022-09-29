
#pragma once

#include <string>

class cExperimentState;


class cExperimentStateCreator
{
public:
    virtual cExperimentState* createState(const std::string& type) = 0;
};


