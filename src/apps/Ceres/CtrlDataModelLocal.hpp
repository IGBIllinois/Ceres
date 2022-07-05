
#pragma once

#include "CtrlDataModel.hpp"
#include "BlockDataFile.hpp"
#include "ExperimentSerializer.hpp"

class cCtrlDataModelLocal : public cCtrlDataModel
{
    Q_OBJECT

public:
    explicit cCtrlDataModelLocal(QObject* parent = nullptr);
    ~cCtrlDataModelLocal();

    void stopDataThread() override;

    bool openDataFile(const QString& defaultPath) override;
    void closeDataFile() override;

    void startExperiment() override;

private:
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    QMutex mMutex;
};

