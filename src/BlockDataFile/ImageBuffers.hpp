
#pragma once


#include <vector>

class cImageBuffer
{
public:
	virtual ~cImageBuffer() = default;

	void clear() { mData.clear(); };

	void resize(std::size_t s) { mData.resize(s); };

	std::size_t size() const { return mData.size(); };

	const std::byte* data() const { return mData.data(); };
	std::byte* data() { return mData.data(); };

protected:
	cImageBuffer() = default;

protected:
	std::vector<std::byte> mData;
};

class cBitmapBuffer : public cImageBuffer
{
public:
	cBitmapBuffer();
	virtual ~cBitmapBuffer();

};

class cJpegBuffer : public cImageBuffer
{
public:
	cJpegBuffer();
	virtual ~cJpegBuffer();

};

class cMpegFrameBuffer : public cImageBuffer
{
public:
	cMpegFrameBuffer();
	virtual ~cMpegFrameBuffer();

};

