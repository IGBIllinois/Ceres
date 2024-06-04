
#pragma once

#include "ExperimentDesignWidget.hpp"

#include "ExperimentFile.hpp"


class cExperimentDesignMdiChild : public cExperimentDesignWidget
{
    Q_OBJECT

public:
    cExperimentDesignMdiChild(QWidget *parent);

    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

protected:
    void closeEvent(QCloseEvent *event) override;

    // Helper slots
private slots:
    void onExperimentChange();
    void onInsertStepBefore(int id, int type);
    void onInsertStepAfter(int id, int type);
    void onDeleteStep(int id);

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;
    bool isUntitled;

    cExperimentFile mExperimentFile;
};

