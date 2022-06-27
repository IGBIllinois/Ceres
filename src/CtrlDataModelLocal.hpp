
#pragma once

#include "DataModel.hpp"
#include "BlockDataFile/BlockDataFile.hpp"
#include "BlockDataFile/ExperimentSerializer.hpp"

class cDataModelLocal : public cDataModel
{
    Q_OBJECT

public:
    explicit cDataModelLocal(QObject* parent = nullptr);
    ~cDataModelLocal();

    void stopDataThread() override;

    bool openDataFile(const QString& defaultPath) override;
    void closeDataFile() override;

    void startExperiment() override;

private:
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    QMutex mMutex;
};

