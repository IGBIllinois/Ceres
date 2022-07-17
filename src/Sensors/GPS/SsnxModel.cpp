
#include "SsnxModel.hpp"
#include "SsnxFactory.hpp"
#include <functional>

cSsnxModel::cSsnxModel(QObject* parent)
:
    cGpsModel("SSNX GPS", parent)
{
}

cSsnxModel::~cSsnxModel()
{
}

char* cSsnxModel::descriptor() const 
{
    return ssnx_id;
};

bool cSsnxModel::configure(const nlohmann::json& jsonCfg)
{
    auto result = cGpsModel::configure(jsonCfg);

    if (!mModel.empty())
        updateName(mModel);

    return result;
}

