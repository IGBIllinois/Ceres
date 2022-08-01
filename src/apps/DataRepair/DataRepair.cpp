
#include "DataRepair.hpp"
#include "BlockDataFileExceptions.hpp"

#include <QDir>
#include <QFileInfo>


//-----------------------------------------------------------------------------
cDataRepair::cDataRepair(const QString& repairedDir, QObject* parent)
    : QObject(parent)
{
    mRepairedDataDirectory = repairedDir;
}

cDataRepair::~cDataRepair()
{}

//-----------------------------------------------------------------------------
bool cDataRepair::open(const std::string& file_name)
{
    mCurrentFileName = QString::fromStdString(file_name);
    QString filename = QFileInfo(mCurrentFileName).fileName();
    mRepairedFileName = mRepairedDataDirectory + filename;

    if (QDir().exists(mRepairedFileName))
        return false;

    std::string repairFilename = mRepairedFileName.toStdString();
    if (!mFileWriter.open(repairFilename))
    {
        return false;
    }

    return cBlockDataFileReader::open(file_name);
}

//-----------------------------------------------------------------------------
void cDataRepair::run()
{
    if (!cBlockDataFileReader::isOpen())
    {
        emit fileResults(false, "File is not open!");
        return;
    }

    try
    {
        while (!eof())
        {
            if (fail())
            {
                emit fileResults(false, "I/O Error: failbit is set.");
                cBlockDataFileReader::close();
                return;
            }

//            try
            {
                cBlockDataFileReader::processBlock();
            }
//            catch (const bdf::crc_error& e)
            {
                // CRC error are not necessarily corrupt files
//                emit statusMessage(e.what());
            }
        }
    }
    catch (const bdf::stream_error& e)
    {
        std::string msg = e.what();
        emit fileResults(false, e.what());
    }
    catch (const std::exception& e)
    {
        if (eof())
        {
            emit fileResults(true, QString());
        }
        else
        {
            emit fileResults(false, e.what());
        }
        cBlockDataFileReader::close();
        return;
    }

    emit fileResults(true, QString());
    cBlockDataFileReader::close();
}

void cDataRepair::processBlock(const cBlockID& id)
{
    mFileWriter.writeBlock(id);
}

void cDataRepair::processBlock(const cBlockID& id, const std::byte* buf, std::size_t len)
{
    mFileWriter.writeBlock(id, buf, len);
}


