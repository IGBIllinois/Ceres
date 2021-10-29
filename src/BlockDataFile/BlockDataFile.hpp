
#pragma once

#include "BlockIdentifiers.hpp"

#include <cstddef>
#include <string>
#include <cstdio>
#include <fstream>

/**
	Storage of the data from the Spidercam system is based on a block file format.

	Data File Signature
	The first eight bytes of the datastream always contain the following(decimal) values:

	137 83 80 68 82 67 77 03

	The next four bytes of the datastream are used as a byte order marker (BOM).  The
	BOM is the hexidecimal number 55AA.  This can be written into file as:

	Big Endian: 55AA	or	Little Endian: AA55


	This signature indicates that the remainder of the datastream contains a single PNG image,
	consisting of a series of chunks beginning with an IHDR chunkand ending with an IEND chunk.

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


class cBlockDataFile
{
public:
	cBlockDataFile();
	explicit cBlockDataFile(const std::string& filename);

	cBlockDataFile(const cBlockDataFile&) = delete;
	cBlockDataFile(cBlockDataFile&&) = delete;

	cBlockDataFile& operator=(const cBlockDataFile&) = delete;
	cBlockDataFile& operator=(cBlockDataFile&&) = delete;

	~cBlockDataFile();

    void open(const std::string& filename);
    bool isOpen();
    void close();

	void writeBlock(BlockID_t id);
	void writeBlock(BlockID_t id, const std::byte* buf, std::size_t len);

private:
//    FILE* mpFile;
    std::ofstream mFile;

	const uint8_t mHeader[8] = {137, 83, 80, 68, 82, 67, 77, 03};
	const uint16_t mBOM = 0x55AA;
};

