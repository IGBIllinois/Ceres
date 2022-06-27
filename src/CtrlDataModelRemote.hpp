
#pragma once

#include "DataModel.hpp"
#include "BlockDataFile/BlockDataFile.hpp"
#include "BlockDataFile/ExperimentSerializer.hpp"


class cDataModelRemote : public cDataModel
{
    Q_OBJECT

public:
    explicit cDataModelRemote(QObject* parent = nullptr);
    ~cDataModelRemote();

    bool openDataFile(const QString& defaultPath) override;
    void closeDataFile();

    void startExperiment() override;

private:
//    cBlockDataFileWriter    mFile;
//    cExperimentSerializer   mSerializer;
};

