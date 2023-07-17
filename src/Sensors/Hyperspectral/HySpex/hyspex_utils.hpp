/*
   Basic utilities for dealing with HySpex cameras
*/

#pragma once

#include <cbdf/HyperspectralBuffers.hpp>
#include <HySpexConnect/HySpexDataBuffer.hpp>


namespace hyspex
{
	template<typename T1, typename T2>
	cHyperspectralImageBuffer<T1> to_image_buffer(const HySpexConnect::spatial_major_data<T2>& rhs);
}
// End of namespace hyspex


/******************************************************************************
 *  I M P L E M E N T A T I O N   D E T A I L S
 *****************************************************************************/

 /////////////////////////////////////////////////////////////////////////////////////
 // View
 /////////////////////////////////////////////////////////////////////////////////////


