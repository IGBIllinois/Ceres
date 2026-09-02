
#pragma once

#include "HySpexCamera_Controller.hpp"


class cHySpexVNIR_3000N_Controller : public cHySpexCamera_Controller
{
	Q_OBJECT

public:
    cHySpexVNIR_3000N_Controller(QObject* parent = nullptr);

public:
    /*** Qt slot handler */


protected:
    /*** Messages handlers from the decoder */


protected:
    void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override;
};


