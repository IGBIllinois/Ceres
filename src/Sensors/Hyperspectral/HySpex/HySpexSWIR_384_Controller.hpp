
#pragma once

#include "HySpexCamera_Controller.hpp"


class cHySpexSWIR_384_Controller : public cHySpexCamera_Controller
{
	Q_OBJECT

public:
    cHySpexSWIR_384_Controller(QObject* parent = nullptr);


public:
    /*** Qt slot handler */
    void onBackgroundComplete(hyspex::BackgroundStatus status) override;

protected:
    /*** Messages handlers from the decoder */


protected:
    void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override;
};


