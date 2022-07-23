
#include "CeresDataFile.hpp"


//-----------------------------------------------------------------------------
cCeresDataFile::cCeresDataFile()
{}

cCeresDataFile::~cCeresDataFile()
{}

//-----------------------------------------------------------------------------
bool cCeresDataFile::open(const std::string& file_name)
{
    mFileReader.open(file_name);

    return mFileReader.isOpen();
}

//-----------------------------------------------------------------------------
bool cCeresDataFile::is_open()
{
    return mFileReader.isOpen();
}

//-----------------------------------------------------------------------------
void cCeresDataFile::close()
{
    if (!mFileReader.isOpen()) return;

    mFileReader.close();
}

void cCeresDataFile::attach(cBlockParser* pParser)
{
    mFileReader.attach(pParser);
}

cBlockParser* cCeresDataFile::detach(const cBlockParser* pParser)
{
    return mFileReader.detach(pParser->blockID());
}

cBlockParser* cCeresDataFile::detach(cBlockID id)
{
    return mFileReader.detach(id);
}

//-----------------------------------------------------------------------------
bool cCeresDataFile::updateData()
{
    if (!mFileReader.processBlock())
    {
        return false;
    }

    return true;
}

