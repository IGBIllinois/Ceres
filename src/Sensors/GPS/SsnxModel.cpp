
#include "SsnxModel.hpp"
#include "SsnxFactory.hpp"
#include <functional>

cSsnxModel::cSsnxModel(QObject* parent)
:
    cGpsModel(parent)
{
}

cSsnxModel::~cSsnxModel()
{
}

char* cSsnxModel::descriptor() const 
{
    return ssnx_id;
};

