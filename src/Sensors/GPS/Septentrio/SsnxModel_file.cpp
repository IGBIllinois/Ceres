
#include "SsnxModel_file.hpp"
#include <functional>

using namespace ssnx;

cSsnxModel_file::cSsnxModel_file(QObject* parent)
:
    cSsnxModel(parent)
{
}

cSsnxModel_file::~cSsnxModel_file()
{
}

bool cSsnxModel_file::configure(const nlohmann::json& jsonCfg)
{
    return true;
}

bool cSsnxModel_file::startCommunications()
{
    return true;
}

void cSsnxModel_file::stopCommunications()
{
}

void cSsnxModel_file::update()
{
}

