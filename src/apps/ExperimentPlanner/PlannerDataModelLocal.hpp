
#pragma once

#include "PlannerDataModel.hpp"

#include <QTimer>


class cPlannerDataModelLocal : public cPlannerDataModel
{
    Q_OBJECT

public:
    explicit cPlannerDataModelLocal(QObject* parent = nullptr);
    ~cPlannerDataModelLocal();

    void stopDataThread() override;

    bool openDataFile(const QString& defaultPath, 
                      const std::string& defaultFileName,
                      bool autoSave) override;

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
    QMutex mMutex;

    QTimer* mpHeartbeatTimer = nullptr;
};

