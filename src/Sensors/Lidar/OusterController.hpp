
#pragma once

#include "../SensorController.hpp"
#include "OusterModel.hpp"


class cOusterController : public cSensorController
{
	Q_OBJECT

public:
    cOusterController(cOusterModel* model, QObject* parent = nullptr);

protected:
    /**
     * @brief processStream
     * The processStream method convert the byte stream from a TCP
     * receiver port.
     */
    void processStream(const void* pBuffer, std::size_t buf_length) override;

private:
    cOusterModel* mpModel = nullptr;
};