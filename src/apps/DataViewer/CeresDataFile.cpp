
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
    if (!mFileReader.isOpen()) return false;

//    mFileReader.attach(static_cast<cExperimentParser*>(this));
//    mFileReader.attach(static_cast<cPvtParser*>(this));

    return true;
}

//-----------------------------------------------------------------------------
void cCeresDataFile::close()
{
    if (!mFileReader.isOpen()) return;

//    mFileReader.detach(static_cast<cExperimentParser*>(this)->blockID());
//    mFileReader.attach(static_cast<cPvtParser*>(this));

    mFileReader.close();
}

void cCeresDataFile::attach(cBlockParser* pParser)
{

}

cBlockParser* cCeresDataFile::detach(cBlockID id)
{

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

