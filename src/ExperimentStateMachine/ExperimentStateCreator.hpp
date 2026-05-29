
#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include <qglobal.h>

class cExperimentState;

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
    class QObject;
QT_END_NAMESPACE


class cExperimentStateCreator
{
public:
    virtual cExperimentState* createState(const std::string& type, const nlohmann::json& stateDoc, QObject* parent) = 0;

protected:
    cExperimentStateCreator() = default;
    virtual ~cExperimentStateCreator() = default;
};


