/**
 * @file
 * @brief Serializer for storing HySpex VNIR 3000N data into a block based data file
 */
#pragma once

#include <cbdf/BlockSerializer.hpp>
#include "HySpexBlockId.hpp"

// Forward Declarations


class cHySpexVNIR_3000N_Serializer : public cBlockSerializer
{
public:
	cHySpexVNIR_3000N_Serializer();
	explicit cHySpexVNIR_3000N_Serializer(std::size_t n, cBlockDataFileWriter* pDataFile);
	~cHySpexVNIR_3000N_Serializer() = default;

protected:
	cBlockID& blockID() override;

private:
	cHySpex_VNIR_3000N_ID       mBlockID;
};

