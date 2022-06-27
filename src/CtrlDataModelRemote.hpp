
#pragma once

#include "CtrlDataModel.hpp"
#include "BlockDataFile/BlockDataFile.hpp"
#include "BlockDataFile/ExperimentSerializer.hpp"


class cCtrlDataModelRemote : public cCtrlDataModel
{
    Q_OBJECT

public:
    explicit cCtrlDataModelRemote(QObject* parent = nullptr);
    ~cCtrlDataModelRemote();

    bool openDataFile(const QString& defaultPath) override;
    void closeDataFile();

    void startExperiment() override;

private:
//    cBlockDataFileWriter    mFile;
//    cExperimentSerializer   mSerializer;
};

