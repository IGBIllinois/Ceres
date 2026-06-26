
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
                      const std::string& defaultFileName,
                      bool autoSave) override;

    bool isDataFileOpen() const override;
    void closeDataFile() override;

    void startExperiment() override;

protected:
    cExperimentState* createState(const std::string& type, const nlohmann::json& stateDoc, QObject* parent) override;

protected:
    bool hasRequiredSensors(const std::string& exp_name, const nlohmann::json& expDoc) override;
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

