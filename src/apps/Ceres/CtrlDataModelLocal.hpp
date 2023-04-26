
#pragma once

#include "CtrlDataModel.hpp"
#include <cbdf/BlockDataFile.hpp>
#include <cbdf/ExperimentSerializer.hpp>

#include <QTimer>


class cCtrlDataModelLocal : public cCtrlDataModel
{
    Q_OBJECT

public:
    explicit cCtrlDataModelLocal(QObject* parent = nullptr);
    ~cCtrlDataModelLocal();

    void stopDataThread() override;

    bool openDataFile(const QString& defaultPath, 
                      const std::string& defaultFileName = std::string()) override;

    bool isDataFileOpen() const override;
    void closeDataFile() override;

    void startExperiment() override;

protected:
    void endDataRecording() override;

    void dataRecordingStateChange(bool record) override;

/*
 * Signals handlers from the timer
 */
private slots:
    void onHeartbeat();


private:
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    QMutex mMutex;

    QTimer* mpHeartbeatTimer = nullptr;
};

