
#include "OusterController.hpp"


cOusterController::cOusterController(cOusterModel* model, QObject* parent)
    :
    cSensorController(parent), mpModel(model)
{}

void cOusterController::processStream(const void* pBuffer, std::size_t buf_length)
{
}

