
#include "SsnxController.hpp"
#include "SsnxModel.hpp"
#include "../GpsControllerNetEncoder.hpp"

#include <cassert>


cSsnxController::cSsnxController(cSsnxModel* pModel, QObject* parent)
    :
    cGpsController(pModel, parent), mpModel(pModel)
{
    assert(mpModel);
}

void cSsnxController::connectToModel()
{
    cGpsController::connectToModel();
}



