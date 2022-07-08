/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once


#include <cstddef>


class cCeresNetDecoder
{
public:

    cCeresNetDecoder() = default;
    virtual ~cCeresNetDecoder() = default;

    /**
     * @brief The decode method convert the byte stream from a TCP receiver port.
     * The decode the various packets and call the various pure virtual 
     * methods.
     */
    void decode(const void* pBuffer, std::size_t buf_length);


private:
};


