/*
 * The HySpexVNIR_3000N_PropertiesNetEncoder is used to encode TCP packets
 * to send to the HySpexVNIR_3000N_ControllerNetDecoder.
 */


#pragma once

#include "HySpexCamera_PropertiesNetEncoder.hpp"
#include <cstddef>


class cHySpexVNIR_3000N_PropertiesNetEncoder : public cHySpexCamera_PropertiesNetEncoder
{

public:
    cHySpexVNIR_3000N_PropertiesNetEncoder(std::size_t capacity);
    ~cHySpexVNIR_3000N_PropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */
};


