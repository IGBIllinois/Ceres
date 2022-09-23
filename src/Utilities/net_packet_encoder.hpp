
#pragma once

#include "net_buffer.hpp"
#include <cstddef>


class cNetworkEncoder
{

public:
    cNetworkEncoder(std::size_t capacity);
    ~cNetworkEncoder() = default;

    /**
     * \brief empty
     * \par Description
     *		Checks if the current the buffer data is empty.
     */
    bool empty() const;

    /**
     * \brief size
     * \par Description
     *		Returns the total length of the buffer data that can be read from.
     *
     * The read size is the diffence between the current write position
     * and the start of the buffer.
     */
    std::size_t size() const;

    /**
     * \brief clear
     * \par Description
     *		Clears the current contents of the of the control sequence.
     */
    void clear();

    /**
     * \brief overrun
     * \par Description
     *		True, if there is not enough space in the data buffer to write more data.
     */
    bool overrun() const;

    /**
     * \brief data
     * \par Description
     *		Returns a pointer to the currently allocated buffer.
     *
     * Note: data returns a const char* for use
     */
    const void* data() const;

protected:
    virtual int sendOutgoingData(const char* data, std::size_t len) = 0;

protected:
    int sendData();

    ceres::net_buffer mBuffer;
};


inline bool cNetworkEncoder::empty() const
{
    return mBuffer.empty();
}

inline std::size_t cNetworkEncoder::size() const
{
    return mBuffer.size();
}

inline void cNetworkEncoder::clear()
{
    mBuffer.clear();
}

inline bool cNetworkEncoder::overrun() const
{
    return mBuffer.overrun();
}

inline const void* cNetworkEncoder::data() const
{
    return mBuffer.data();
}
