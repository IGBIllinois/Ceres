/*
 * The HySpexSWIR_384_ControllerNetEncoder is used to encode TCP packets
 * to the HySpexSWIR_384_PropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_ControllerNetEncoder.hpp"


class cHySpexSWIR_384_ControllerNetEncoder : public cHySpexCamera_ControllerNetEncoder
{

public:
    cHySpexSWIR_384_ControllerNetEncoder(std::size_t capacity);
    ~cHySpexSWIR_384_ControllerNetEncoder();

public:
    /*
     * Controller ----> Property Page
     */
};


