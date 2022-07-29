/**
 * @file
 */
#pragma once

#include "BlockDataFile.hpp"

#include <QObject>
#include <QRunnable>

class cDataVerifier : public QObject, public QRunnable
{
    Q_OBJECT

public:
    cDataVerifier(QObject* parent = nullptr);
    ~cDataVerifier();

    bool open(const std::string& file_name);

    void run() override;

signals:
    void fileResults(bool valid, QString msg);

private:
    cBlockDataFileReader mFileReader;
};

