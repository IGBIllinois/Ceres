
#include "net_buffer.hpp"

namespace
{
	/**
	 * \brief read
	 * \par Description
	 *		A template member function to read a variable from the internal buffer.
	 *
	 * \param X		-- a reference to the parameter to read from the buffer.
	 */
	template<typename T>
	std::size_t read(T& X, const std::byte* start, const std::byte* end, bool& underrun)
	{
		std::size_t len = std::distance(start, end);

		// Check to make sure we have enough buffer space to put this variable into
		// our internal storage.
		if ((start >= end) || (len < sizeof(T)))
		{
			underrun = true;
			return 0;
		}

//		if (mByteOrder == enumByteOrder_LITTLE_ENDIAN)
//		{
			X = *(reinterpret_cast<const T*>(start));
//		}
//		else
		{
//			unpack_s(X, start, mCapacity, mReadIndex, true);
		}

		return sizeof(T);
	}

	/**
	 * \brief write
	 * \par Description
	 *		A template member function to write a variable into the internal buffer.
	 *
	 * \param X		-- the parameter to write into the buffer.
	 */
	template<typename T>
	std::size_t write(const T& X, std::byte* start, const std::byte* end, bool& overrun)
	{
		// Check to make sure we have enough buffer space to put this variable into
		// our internal storage.
		std::size_t free = std::distance<const std::byte*>(start, end);
		if (free < sizeof(T))
		{
			overrun = true;
			return 0;
		}

//		if (mByteOrder == enumByteOrder_LITTLE_ENDIAN)
		{
			// copy the host order value into the buffer
			memcpy(start, &X, sizeof(T));
		}
//		else
		{
//			pack_s(X, mpBuffer, mCapacity, mWriteIndex, true);
		}

		return sizeof(T);
	};

}

using namespace ceres;

net_buffer::net_buffer() 
: mpBuffer(nullptr), mReadIndex(0), mWriteIndex(0), mCapacity(0), 
	mUnderrun(false), mOverrun(false)
{
}

net_buffer::net_buffer(size_type capacity)
: mpBuffer(nullptr), mReadIndex(0), mWriteIndex(0), mCapacity(0),
	mUnderrun(true), mOverrun(false)
{
	mpBuffer = new std::byte[capacity];

	if (mpBuffer != nullptr)
	{
		mCapacity = capacity;
	}
}

net_buffer::net_buffer(const net_buffer& objToCopy)
	: mpBuffer(nullptr), mReadIndex(0), 
	mWriteIndex(objToCopy.mWriteIndex),
	mCapacity(objToCopy.mCapacity),
	mUnderrun(objToCopy.mUnderrun), mOverrun(objToCopy.mOverrun)
{
	mpBuffer = new std::byte[objToCopy.mCapacity];

	memcpy(&mpBuffer[0], objToCopy.mpBuffer, mCapacity);
}

net_buffer::net_buffer(net_buffer&& objToMove) noexcept
{
	mpBuffer = objToMove.mpBuffer;
	mCapacity = objToMove.mCapacity;
	mReadIndex = objToMove.mReadIndex;
	mWriteIndex = objToMove.mWriteIndex;
	mUnderrun = objToMove.mUnderrun;
	mOverrun = objToMove.mOverrun;

	objToMove.mpBuffer = nullptr;
	objToMove.mCapacity = 0;
	objToMove.mReadIndex = 0;
	objToMove.mWriteIndex = 0;
	objToMove.mUnderrun = false;
	objToMove.mOverrun = false;
}

void net_buffer::operator=(const net_buffer& objToCopy)
{
	delete[] mpBuffer;
	mpBuffer = nullptr;
	mpBuffer = new std::byte[objToCopy.mCapacity];

	if (mpBuffer != nullptr)
	{
		mCapacity = objToCopy.mCapacity;
	}

	mReadIndex = 0;
	mWriteIndex = objToCopy.mWriteIndex;
	mUnderrun = objToCopy.mUnderrun;
	mOverrun = objToCopy.mOverrun;

	memcpy(&mpBuffer[0], objToCopy.mpBuffer, mCapacity);
}

net_buffer::~net_buffer()
{
	delete[] mpBuffer;
	mpBuffer = nullptr;

	mReadIndex = 0;
	mWriteIndex = 0;
	mCapacity = 0;
	mUnderrun = false;
	mOverrun = false;
}

net_buffer::operator net_buffer_view()
{
	return net_buffer_view(rdbuf(), mWriteIndex-mReadIndex);
}

std::size_t net_buffer::write_size() const
{
	if (mWriteIndex > mCapacity) return 0;
	return mCapacity - mWriteIndex;
}

std::size_t net_buffer::read_size() const
{
	return mWriteIndex - mReadIndex;
}


std::size_t net_buffer::size() const
{
	return mWriteIndex;
}

std::size_t net_buffer::capacity() const
{
	return mCapacity;
}

void net_buffer::capacity(size_type capacity)
{
	if (mpBuffer == nullptr)
	{
		mpBuffer = new std::byte[capacity];

		if (mpBuffer != nullptr)
		{
			mCapacity = capacity;
		}

		return;
	}

	if (capacity > mCapacity)
	{
		std::byte* pBuffer = new std::byte[capacity];

		if (pBuffer != nullptr)
		{
			memcpy(pBuffer, mpBuffer, mCapacity);

			delete[] mpBuffer;

			mpBuffer = pBuffer;
			mCapacity = capacity;
			mUnderrun = false;
			mOverrun = false;
		}

		return;
	}

	std::byte* pBuffer = new std::byte[capacity];

	if (pBuffer != nullptr)
	{
		reset();

		delete[] mpBuffer;

		mpBuffer = pBuffer;
		mCapacity = capacity;
	}
}

bool net_buffer::empty() const
{
	return (mpBuffer == nullptr) || (mWriteIndex == 0);
}

bool net_buffer::underrun() const
{
	return mUnderrun;
}

bool net_buffer::overrun() const
{
	return mOverrun;
}

const void* net_buffer::data() const
{
	return mpBuffer;
}

void net_buffer::attach(std::byte* buffer, size_type size)
{
	delete[] mpBuffer;

	mpBuffer = buffer;
	mCapacity = size;
	reset();
	mWriteIndex = size;
}

std::byte* net_buffer::detach()
{
	std::byte* retPtr = mpBuffer;

	// Clear our internal pointer and reset the indexes
	mpBuffer = nullptr;
	mCapacity = 0;

	reset();

	return retPtr;
}

void net_buffer::reset()
{
	mReadIndex = 0;
	mWriteIndex = 0;
	mUnderrun = false;
	mOverrun = false;
}

void net_buffer::clear()
{
	reset();

	if (mpBuffer != nullptr)
		memset(mpBuffer, 0, mCapacity);
}

net_buffer::size_type net_buffer::insert(const unsigned char* buffer, size_type size)
{
	if (size > write_size())
	{
		mOverrun = true;
		size = write_size();
	}

	memcpy(&mpBuffer[mWriteIndex], buffer, size);
	mWriteIndex += size;
	return size;
}

void net_buffer::padTo(size_type size, std::byte value)
{
	if (size > mWriteIndex)
	{
		std::size_t newBytes = size - mWriteIndex;

		if (newBytes > write_size())
		{
			mOverrun = true;
			newBytes = write_size();
		}

		if (newBytes > 0)
		{
			memset(mpBuffer + mWriteIndex, static_cast<unsigned char>(value), newBytes);
			mWriteIndex += newBytes;
		}
	}
}

std::size_t net_buffer::erase(std::size_t pos, size_type size)
{
	// If the start position is passed the current size of the buffer we can't
	// erase any elements.
	if (pos > mWriteIndex)
		return 0;

	// If the size is more then the number of element left in the control
	// sequence, zero fill the remaining in sequence.
	if (size > (mWriteIndex - pos))
	{
		size = mWriteIndex - pos;
		memset(&mpBuffer[pos], 0, size);
		mWriteIndex = pos;
		return size;
	}

	memcpy(&mpBuffer[pos], &mpBuffer[pos + size], mWriteIndex - (pos + size));
	mWriteIndex -= size;
	memset(&mpBuffer[mWriteIndex], 0, size);
	return size;
}

///////////////////////////////////////////////////////////////////////////////
// Input Stream Operators
///////////////////////////////////////////////////////////////////////////////

net_buffer& net_buffer::operator>>(net_buffer& out)
{
	out << *this;
	return *this;
}

net_buffer& net_buffer::operator>>(net_buffer*& out)
{
	(*out) << *this;
	return *this;
}

net_buffer& net_buffer::operator>>(bool& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(char& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(int8_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(uint8_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(int16_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(uint16_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(int32_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(uint32_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(int64_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(uint64_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(float& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

net_buffer& net_buffer::operator>>(double& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

void net_buffer::read(std::string& out, unsigned char len)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if ((mReadIndex >= mWriteIndex) || (read_size() < len))
	{
		mUnderrun = true;
		return;
	}

	out.clear();

	// copy the string from the buffer
	out.assign(reinterpret_cast<char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;
}

void net_buffer::read(std::byte*& out, unsigned char len)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if ((mReadIndex >= mWriteIndex) || (read_size() < len))
	{
		mUnderrun = true;
		return;
	}

	// copy the string from the buffer
	memcpy(out, reinterpret_cast<char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;
}

///////////////////////////////////////////////////////////////////////////////
// Output Stream Operators
///////////////////////////////////////////////////////////////////////////////

net_buffer& net_buffer::operator<<(const net_buffer& in)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	std::size_t size = in.read_size();
	if (write_size() < size)
	{
		mOverrun = true;
		return *this;
	}

	memcpy(&mpBuffer[mWriteIndex], in.rdbuf(), size);
	mWriteIndex += size;
	return *this;
}

net_buffer& net_buffer::operator<<(const net_buffer*& in)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	std::size_t size = in->read_size();
	if (write_size() < size)
	{
		mOverrun = true;
		return *this;
	}

	memcpy(&mpBuffer[mWriteIndex], in->rdbuf(), size);
	mWriteIndex += size;
	return *this;
}

net_buffer& net_buffer::operator<<(const bool in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const char in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const int8_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const uint8_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const int16_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const uint16_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const int32_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const uint32_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const int64_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const uint64_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const float in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

net_buffer& net_buffer::operator<<(const double in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

void net_buffer::write(const std::string& in)
{
	if (in.empty())
	{
		return;
	}

	auto len = in.length();

	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if (write_size() < len)
	{
		mOverrun = true;
		return;
	}

	// put the string into the buffer
	memcpy(&mpBuffer[mWriteIndex], in.data(), len);

	mWriteIndex += len;
}

void net_buffer::write(const std::byte* in, unsigned char len)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if (write_size() < len)
	{
		mOverrun = true;
		return;
	}

	if (in == nullptr)
	{
		// null out the char array in the buffer
		memset(&mpBuffer[mWriteIndex], 0, len);
		mWriteIndex += len;
		return;
	}

	// put the string into the buffer
	memcpy(&mpBuffer[mWriteIndex], in, len);

	mWriteIndex += len;
}


///////////////////////////////////////////////////////////////////////////////
//
// Net Buffer View
//
///////////////////////////////////////////////////////////////////////////////


net_buffer_view::net_buffer_view()
:
	mpBuffer(nullptr), mReadIndex(0), mCapacity(0), mUnderrun(false)
{}

net_buffer_view::net_buffer_view(const std::byte* buffer, size_type length)
:
	mpBuffer(buffer), mReadIndex(0), mCapacity(length), mUnderrun(false)
{}

net_buffer_view::net_buffer_view(const net_buffer_view& objToCopy)
:
	mpBuffer(objToCopy.mpBuffer), mReadIndex(objToCopy.mReadIndex), 
	mCapacity(objToCopy.mCapacity), mUnderrun(objToCopy.mUnderrun)
{}

net_buffer_view::net_buffer_view(net_buffer_view&& objToMove) noexcept
:
	mpBuffer(objToMove.mpBuffer), mReadIndex(objToMove.mReadIndex),
	mCapacity(objToMove.mCapacity), mUnderrun(objToMove.mUnderrun)
{
	objToMove.mpBuffer = nullptr;
	objToMove.mReadIndex = 0;
	objToMove.mCapacity = 0;
	objToMove.mUnderrun = false;
}

void net_buffer_view::operator=(const net_buffer_view& objToCopy)
{
	mpBuffer = objToCopy.mpBuffer;
	mReadIndex = objToCopy.mReadIndex;
	mCapacity = objToCopy.mCapacity;
	mUnderrun = objToCopy.mUnderrun;
}

net_buffer_view::size_type net_buffer_view::size() const
{
	return mCapacity - mReadIndex;
}

net_buffer_view::size_type net_buffer_view::capacity() const
{
	return mCapacity;
}

bool net_buffer_view::empty() const
{
	return (mpBuffer == nullptr) || (mReadIndex >= mCapacity);
}

bool net_buffer_view::underrun() const
{
	return mUnderrun;
}

const void* net_buffer_view::data() const
{
	return rdbuf();
}

void net_buffer_view::reset()
{
	mReadIndex = 0;
	mUnderrun = false;
}

void net_buffer_view::advance(size_type n)
{
	mReadIndex += n;
	if (mReadIndex >= mCapacity)
	{
		mUnderrun = true;
		mReadIndex = mCapacity;
	}
}

net_buffer_view& net_buffer_view::operator>>(net_buffer_view& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(net_buffer_view * &out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(bool& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(char& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(int8_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(uint8_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(int16_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(uint16_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(int32_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(uint32_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(int64_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(uint64_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(float& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

net_buffer_view& net_buffer_view::operator>>(double& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

void net_buffer_view::read(std::string& out, unsigned char len)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if ((mReadIndex >= mCapacity) || (size() < len))
	{
		mUnderrun = true;
		return;
	}

	out.clear();

	// copy the string from the buffer
	out.assign(reinterpret_cast<const char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;
}

void net_buffer_view::read(std::byte*& out, unsigned char len)
{
	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if ((mReadIndex >= mCapacity) || (size() < len))
	{
		mUnderrun = true;
		return;
	}

	// copy the string from the buffer
	memcpy(out, reinterpret_cast<const char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;

}

