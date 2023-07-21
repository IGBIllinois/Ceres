/*
 * The HySpexVNIR_3000N_ControllerNetEncoder is used to encode TCP packets
 * to the HySpexVNIR_3000N_PropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_ControllerNetEncoder.hpp"


class cHySpexVNIR_3000N_ControllerNetEncoder : public cHySpexCamera_ControllerNetEncoder
{

public:
    cHySpexVNIR_3000N_ControllerNetEncoder(std::size_t capacity);
    ~cHySpexVNIR_3000N_ControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
};


