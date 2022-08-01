/**
 * @file
 */
#pragma once

#include "BlockDataFile.hpp"

#include <QObject>
#include <QRunnable>

class cDataRepair : public QObject, public QRunnable, private cBlockDataFileReader
{
    Q_OBJECT

public:
	cDataRepair(const QString& repairedDir, QObject* parent = nullptr);
    ~cDataRepair();

    bool open(const std::string& file_name);

    void run() override;

signals:
    void statusMessage(QString msg);
    void fileResults(bool valid, QString msg);

private:
    void processBlock(const cBlockID& id) override;
    void processBlock(const cBlockID& id, const std::byte* buf, std::size_t len) override;

private:
    cBlockDataFileWriter mFileWriter;

    QString     mRepairedDataDirectory;
    QString     mCurrentFileName;
    QString     mRepairedFileName;
};

