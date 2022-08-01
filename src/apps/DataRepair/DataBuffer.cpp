
#include "DataBuffer.hpp"

namespace
{
	template <typename T>
	T byte_swap(T b)
	{
		if (sizeof(T) == 1)
			return b;
	}

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


///////////////////////////////////////////////////////////////////////////////
//
// Data Buffer
//
///////////////////////////////////////////////////////////////////////////////


cDataBuffer::cDataBuffer()
: mpBuffer(nullptr), mReadIndex(0), mWriteIndex(0), mCapacity(0), 
	mUnderrun(false), mOverrun(false)
{
}

cDataBuffer::cDataBuffer(size_type capacity)
: mpBuffer(nullptr), mReadIndex(0), mWriteIndex(0), mCapacity(0),
	mUnderrun(true), mOverrun(false)
{
	mpBuffer = new std::byte[capacity];

	if (mpBuffer != nullptr)
	{
		mCapacity = capacity;
	}
}

cDataBuffer::cDataBuffer(const cDataBuffer& objToCopy)
	: mpBuffer(nullptr), mReadIndex(0), 
	mWriteIndex(objToCopy.mWriteIndex),
	mCapacity(objToCopy.mCapacity),
	mUnderrun(objToCopy.mUnderrun), mOverrun(objToCopy.mOverrun)
{
	mpBuffer = new std::byte[objToCopy.mCapacity];

	memcpy(&mpBuffer[0], objToCopy.mpBuffer, mCapacity);
}

cDataBuffer::cDataBuffer(cDataBuffer&& objToMove) noexcept
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

void cDataBuffer::operator=(const cDataBuffer& objToCopy)
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

cDataBuffer::~cDataBuffer()
{
	delete[] mpBuffer;
	mpBuffer = nullptr;

	mReadIndex = 0;
	mWriteIndex = 0;
	mCapacity = 0;
	mUnderrun = false;
	mOverrun = false;
}

cDataBuffer::operator cDataBufferView()
{
	return cDataBufferView(rdbuf(), mWriteIndex-mReadIndex);
}

std::size_t cDataBuffer::write_size() const
{
	if (mWriteIndex > mCapacity) return 0;
	return mCapacity - mWriteIndex;
}

std::size_t cDataBuffer::read_size() const
{
	return mWriteIndex - mReadIndex;
}


std::size_t cDataBuffer::size() const
{
	return mWriteIndex;
}

std::size_t cDataBuffer::capacity() const
{
	return mCapacity;
}

void cDataBuffer::capacity(size_type capacity)
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

bool cDataBuffer::empty() const
{
	return (mpBuffer == nullptr) || (mWriteIndex == 0);
}

bool cDataBuffer::underrun() const
{
	return mUnderrun;
}

bool cDataBuffer::overrun() const
{
	return mOverrun;
}

const std::byte* cDataBuffer::data() const
{
	return mpBuffer;
}

const std::byte* cDataBuffer::data(std::size_t offset) const
{
	return mpBuffer + offset;
}

std::byte* cDataBuffer::data(std::size_t len)
{
	mWriteIndex += len;
	return mpBuffer;
}

void cDataBuffer::attach(std::byte* buffer, std::size_t size)
{
	delete[] mpBuffer;

	mpBuffer = buffer;
	mCapacity = size;
	reset();
	mWriteIndex = size;
}

std::byte* cDataBuffer::detach()
{
	std::byte* retPtr = mpBuffer;

	// Clear our internal pointer and reset the indexes
	mpBuffer = nullptr;
	mCapacity = 0;

	reset();

	return retPtr;
}

void cDataBuffer::reset()
{
	mReadIndex = 0;
	mWriteIndex = 0;
	mUnderrun = false;
	mOverrun = false;
}

void cDataBuffer::clear()
{
	reset();

	if (mpBuffer != nullptr)
		memset(mpBuffer, 0, mCapacity);
}

std::size_t cDataBuffer::insert(const unsigned char* buffer, std::size_t size)
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

void cDataBuffer::padTo(std::size_t size, std::byte value)
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

std::size_t cDataBuffer::erase(std::size_t pos, std::size_t size)
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

cDataBuffer& cDataBuffer::operator>>(cDataBuffer& out)
{
	out << *this;
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(cDataBuffer*& out)
{
	(*out) << *this;
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(bool& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(char& out)
{
	uint8_t ch;
	mReadIndex += ::read(ch, rdbuf(), wrtbuf(), mUnderrun);
	out = ch;
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(int8_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(uint8_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(int16_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(uint16_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(int32_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(uint32_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(int64_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(uint64_t& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(float& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(double& out)
{
	mReadIndex += ::read(out, rdbuf(), wrtbuf(), mUnderrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator>>(std::string& out)
{
	read(out);
	return *this;
}

void cDataBuffer::read(std::string& out)
{
	if (read_size() == 0)
	{
		// In earlier versions, an empty string was written with
		// zero data.  Now, a length of zero is written into the
		// data buffer.  This is to support the older version.
		out.clear();
		return;
	}

	uint16_t len = 0;
	mReadIndex += ::read(len, rdbuf(), wrtbuf(), mUnderrun);

	if (mUnderrun)
		return;

	// Check to make sure we have enough data in the read size of the buffer
	// to fulfill the read request.
	if ((mReadIndex > mWriteIndex) || (read_size() < len))
	{
		mUnderrun = true;
		return;
	}

	out.clear();

	if (len > 0)
	{
		out.resize(len);

		// copy the string from the buffer
		out.assign(reinterpret_cast<char*>(&mpBuffer[mReadIndex]), len);
	}

	mReadIndex += len;
}

void cDataBuffer::read(std::string& out, uint16_t len)
{
	// Check to make sure we have enough data in the read size of the buffer
	// to fulfill the read request.
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

void cDataBuffer::read(std::byte*& out, uint16_t len)
{
	// Check to make sure we have enough data in the read size of the buffer
	// to fulfill the read request.
	if ((mReadIndex >= mWriteIndex) || (read_size() < len))
	{
		mUnderrun = true;
		return;
	}

	// copy the byte block from the buffer
	memcpy(out, reinterpret_cast<char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;
}

void cDataBuffer::read(char* out, std::size_t len)
{
	// Check to make sure we have enough data in the read size of the buffer
	// to fulfill the read request.
	if ((mReadIndex >= mWriteIndex) || (read_size() < len))
	{
		mUnderrun = true;
		return;
	}

	// copy the char block from the buffer
	memcpy(out, reinterpret_cast<char*>(&mpBuffer[mReadIndex]), len);

	mReadIndex += len;
}

///////////////////////////////////////////////////////////////////////////////
// Output Stream Operators
///////////////////////////////////////////////////////////////////////////////

cDataBuffer& cDataBuffer::operator<<(const cDataBuffer& in)
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

cDataBuffer& cDataBuffer::operator<<(const cDataBuffer*& in)
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

cDataBuffer& cDataBuffer::operator<<(const bool in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const char in)
{
	mWriteIndex += ::write<uint8_t>(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const int8_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const uint8_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const int16_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const uint16_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const int32_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const uint32_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const int64_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const uint64_t in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const float in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const double in)
{
	mWriteIndex += ::write(in, wrtbuf(), end(), mOverrun);
	return *this;
}

cDataBuffer& cDataBuffer::operator<<(const std::string& in)
{
	write(in);
	return *this;
}

void cDataBuffer::write(const std::string& in)
{
	uint16_t len = 0;
	if (in.length() > 65533)
		len = 65533;
	else
		len = in.length();

	// Check to make sure we have enough buffer space to put this variable into
	// our internal storage.
	if (write_size() < (len + sizeof(len)))
	{
		mOverrun = true;
		return;
	}

	// put the string length into the data buffer
	mWriteIndex += ::write(len, wrtbuf(), end(), mOverrun);

	if (len > 0)
	{
		// put the string into the buffer
		memcpy(&mpBuffer[mWriteIndex], in.data(), len);
	}

	mWriteIndex += len;
}

void cDataBuffer::write(const std::byte* in, uint16_t len)
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

void cDataBuffer::write(const char* in, std::size_t len)
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

	// put the data into the buffer
	memcpy(&mpBuffer[mWriteIndex], in, len);

	mWriteIndex += len;
}


///////////////////////////////////////////////////////////////////////////////
//
// Data Buffer View
//
///////////////////////////////////////////////////////////////////////////////


cDataBufferView::cDataBufferView()
:
	mpBuffer(nullptr), mReadIndex(0), mCapacity(0), mUnderrun(false)
{}

cDataBufferView::cDataBufferView(const std::byte* buffer, size_type length)
:
	mpBuffer(buffer), mReadIndex(0), mCapacity(length), mUnderrun(false)
{}

cDataBufferView::cDataBufferView(const cDataBufferView& objToCopy)
:
	mpBuffer(objToCopy.mpBuffer), mReadIndex(objToCopy.mReadIndex), 
	mCapacity(objToCopy.mCapacity), mUnderrun(objToCopy.mUnderrun)
{}

cDataBufferView::cDataBufferView(cDataBufferView&& objToMove) noexcept
:
	mpBuffer(objToMove.mpBuffer), mReadIndex(objToMove.mReadIndex),
	mCapacity(objToMove.mCapacity), mUnderrun(objToMove.mUnderrun)
{
	objToMove.mpBuffer = nullptr;
	objToMove.mReadIndex = 0;
	objToMove.mCapacity = 0;
	objToMove.mUnderrun = false;
}

void cDataBufferView::operator=(const cDataBufferView& objToCopy)
{
	mpBuffer = objToCopy.mpBuffer;
	mReadIndex = objToCopy.mReadIndex;
	mCapacity = objToCopy.mCapacity;
	mUnderrun = objToCopy.mUnderrun;
}

cDataBufferView::size_type cDataBufferView::size() const
{
	return mCapacity - mReadIndex;
}

cDataBufferView::size_type cDataBufferView::capacity() const
{
	return mCapacity;
}

bool cDataBufferView::empty() const
{
	return (mpBuffer == nullptr) || (mReadIndex >= mCapacity);
}

bool cDataBufferView::underrun() const
{
	return mUnderrun;
}

const std::byte* cDataBufferView::data() const
{
	return rdbuf();
}

void cDataBufferView::reset()
{
	mReadIndex = 0;
	mUnderrun = false;
}

void cDataBufferView::advance(size_type n)
{
	mReadIndex += n;
	if (mReadIndex >= mCapacity)
	{
		mUnderrun = true;
		mReadIndex = mCapacity;
	}
}

cDataBufferView& cDataBufferView::operator>>(cDataBufferView& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(cDataBufferView* &out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(bool& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(char& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(int8_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(uint8_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(int16_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(uint16_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(int32_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(uint32_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(int64_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(uint64_t & out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(float& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

cDataBufferView& cDataBufferView::operator>>(double& out)
{
	mReadIndex += ::read(out, rdbuf(), end(), mUnderrun);
	return *this;
}

void cDataBufferView::read(std::string& out, unsigned char len)
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

void cDataBufferView::read(std::byte*& out, unsigned char len)
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

