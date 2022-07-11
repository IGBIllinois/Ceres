
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
    bool isDataFileOpen() const override;
    void closeDataFile() override;

    void startExperiment() override;

protected:
    void endDataRecording() override;

    void dataRecordingStateChange(bool record) override;

private:
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    QMutex mMutex;
};

