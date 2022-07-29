
#include "DataVerifier.hpp"


//-----------------------------------------------------------------------------
cDataVerifier::cDataVerifier(QObject* parent)
    : QObject(parent)
{
//    setAutoDelete(false);
}

cDataVerifier::~cDataVerifier()
{}

//-----------------------------------------------------------------------------
bool cDataVerifier::open(const std::string& file_name)
{
    mFileReader.open(file_name);

    return mFileReader.isOpen();
}

//-----------------------------------------------------------------------------
void cDataVerifier::run()
{
    if (!mFileReader.isOpen())
    {
        emit fileResults(false, "File is not open!");
        return;
    }

    try
    {
        while (!mFileReader.eof())
        {
            if (mFileReader.fail())
            {
                emit fileResults(false, "I/O Error: failbit is set.");
                mFileReader.close();
                return;
            }

            mFileReader.processBlock();
        }
    }
    catch (const std::exception& e)
    {
        if (mFileReader.eof())
        {
            emit fileResults(true, QString());
        }
        else
        {
            emit fileResults(false, e.what());
        }
        mFileReader.close();
        return;
    }

    emit fileResults(true, QString());
    mFileReader.close();
}

