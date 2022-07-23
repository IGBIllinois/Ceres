/**
 * @file
 */
#pragma once

#include "BlockDataFile.hpp"

#include <string>

class cCeresDataFile
{
    
public:
    cCeresDataFile();
    ~cCeresDataFile();

    bool open(const std::string& file_name);
    bool is_open();
    void close();

    void attach(cBlockParser* pParser);
    cBlockParser* detach(const cBlockParser* pParser);
    cBlockParser* detach(cBlockID id);

    bool updateData();


private:
    cBlockDataFileReader mFileReader;
};

