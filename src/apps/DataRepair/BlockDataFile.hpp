
#pragma once

#include "BlockId.hpp"
#include "DataBuffer.hpp"

#include <cstddef>
#include <string>
#include <cstdio>
#include <fstream>
#include <map>
#include <mutex>

// Forward Declares

/**
	Storage of the data from the Spidercam system is based on a block file format.

	Data File Signature
	The first eight bytes of the datastream always contain the following(decimal) values:

	137 83 80 68 82 67 77 03

	The next four bytes of the datastream are used as a byte order marker (BOM).  The
	BOM is the hexidecimal number 55AA.  This can be written into file as:

	Big Endian: 55AA	or	Little Endian: AA55


	This signature indicates that the remainder of the datastream consisting of a series
	of data blocks.

	Block Layout
	Each block consists of three or four fields:

	+----------+  +--------------+  +--------------+  +-------+
	|  Length  |  |  Block Type  |  |  Block Data  |  |  CRC  |
	+----------+  +--------------+  +--------------+  +-------+

	or

	+----------+  +--------------+  +-------+
	|  Length  |  |  Block Type  |  |  CRC  |
	+----------+  +--------------+  +-------+


	The block data field may be empty.

	Length		: A four-byte unsigned integer giving the number of bytes in the block's data field.
				  The length counts only the data field, not itself, the block type, or the CRC.
				  Zero is a valid length. Although encoders and decoders should treat the length
				  as unsigned, its value shall not exceed 2^31-1 bytes.

	Block Type	: A four-byte unsigned integer defining the block type.  See the enumerations in DataFileBlockIdentifiers.hpp.

	Block Data	: The data bytes appropriate to the block type, if any. This field can be of zero length.

	CRC			: A four-byte CRC (Cyclic Redundancy Code) calculated on the preceding bytes in the block,
				  including the block type field and block data fields, but not including the length field.
				  The CRC can be used to check for corruption of the data.  The CRC is always present, even
				  for blocks containing no data.
*/


class cBlockDataFileWriter
{
public:
	cBlockDataFileWriter();
	explicit cBlockDataFileWriter(const std::string& filename);

	cBlockDataFileWriter(const cBlockDataFileWriter&) = delete;
	cBlockDataFileWriter(cBlockDataFileWriter&&) = delete;

	cBlockDataFileWriter& operator=(const cBlockDataFileWriter&) = delete;
	cBlockDataFileWriter& operator=(cBlockDataFileWriter&&) = delete;

	~cBlockDataFileWriter();

    bool open(const std::string& filename);
    bool isOpen() const;
    void close();

	bool fail() const;
	bool bad() const;
	bool good() const;

	bool writeBlock(const cBlockID& id);
	bool writeBlock(const cBlockID& id, const std::byte* buf, std::size_t len);

private:
    std::ofstream mFile;
	std::mutex mWriteMutex;
};



class cBlockDataFileReader
{
public:
	cBlockDataFileReader();
	explicit cBlockDataFileReader(const std::string& filename);

	cBlockDataFileReader(const cBlockDataFileReader&) = delete;
	cBlockDataFileReader(cBlockDataFileReader&&) = delete;

	cBlockDataFileReader& operator=(const cBlockDataFileReader&) = delete;
	cBlockDataFileReader& operator=(cBlockDataFileReader&&) = delete;

	~cBlockDataFileReader();

	bool open(const std::string& filename);
	bool isOpen() const;
	void close();

	bool fail() const;
	bool bad() const;
	bool good() const;
	bool eof() const;

	bool processBlock();

	virtual void processBlock(const cBlockID& id) = 0;
	virtual void processBlock(const cBlockID& id, const std::byte* buf, std::size_t len) = 0;

private:
	void readPayload(uint32_t len, cDataBuffer& buffer);
	uint32_t readCRC();

	bool tryToFixBlockId(const cBlockID blockID);
	bool tryToFixBlock(const cBlockID blockID, const cDataBuffer buffer, const uint32_t len);

	enum class eBlockStatus { OK, BAD_CLASS_ID, BAD_MAJOR_VERSION, BAD_MINOR_VERSION, BAD_DATA_ID, BAD_PAYLOAD };
	eBlockStatus checkBlockId(const cBlockID blockID, uint32_t len);

private:
	bool fixAtBlockId(const cBlockID originalBlockID, uint32_t originalLen, eBlockStatus blockStatus);
	bool fixAtDataBuffer();

	bool fixAtCRC(const cBlockID originalBlockID, 
		const cBlockID insertedBlockID, uint32_t insertedLen);

	bool fixAtCRC(const cBlockID originalBlockID, const cDataBuffer originalBuffer,
		uint32_t originalLen, const cBlockID insertedBlockID, uint32_t insertedLen);

	bool recoverBlockID(cBlockID& blockID, uint32_t len, eBlockStatus blockStatus);

private:
	eBlockStatus checkExperimentBlock(const cBlockID blockID, uint32_t len);
	eBlockStatus checkAxisCommunicationBlock(const cBlockID blockID, uint32_t len);
	eBlockStatus checkOusterLidarBlock(const cBlockID blockID, uint32_t len);
	eBlockStatus checkSpidercamBlock(const cBlockID blockID, uint32_t len);
	eBlockStatus checkSsnxBlock(const cBlockID blockID, uint32_t len);
	eBlockStatus checkWeatherBlock(const cBlockID blockID, uint32_t len);

private:
	std::ifstream mFile;
	std::ifstream::pos_type mStartOfBlock;
	std::ifstream::pos_type mStartOfClassID;
	std::ifstream::pos_type mStartOfMajorVersion;
	std::ifstream::pos_type mStartOfMinorVersion;
	std::ifstream::pos_type mStartOfDataID;
	std::ifstream::pos_type mStartOfPayload;
	std::ifstream::pos_type mStartOfCRC;

	bool mByteSwapNeeded;

	cDataBuffer mBuffer;
};

