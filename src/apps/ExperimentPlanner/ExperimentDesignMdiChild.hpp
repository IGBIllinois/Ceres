
#pragma once

#include "ExperimentDesignWidget.hpp"

#include "ExperimentFile.hpp"

#include <string>


class cExperimentDesignMdiChild : public cExperimentDesignWidget
{
    Q_OBJECT

public:
    cExperimentDesignMdiChild(QWidget *parent);

    void newFile();
    void newFile(const cExperimentFile& file);
    void loadFile(const QString &fileName);
    void save();
    void saveAs();

    QString userFriendlyCurrentFile();
    QString currentFile();

    const std::string& getFileName() const;

    const std::string& getExperimentTitle() const;

    const cExperimentFile& getExperimentFile() const;
    void setExperimentFile(const cExperimentFile& file);

    const std::string& getLayoutName() const;
    void setLayoutName(const std::string& name);

    void editMetaInfo();
    void editCtrlInfo();
    void editSensorInfo();

    void set_X_Position(int x_mm);
    void set_Y_Position(int y_mm);
    void set_Z_Position(int z_mm);

    void shiftPositions(int x_mm, int y_mm, int z_mm);

    void reloadPath();

signals:
    void experimentListNeedsUpdate();

protected:
    void closeEvent(QCloseEvent *event) override;
    void focusInEvent(QFocusEvent* event) override;

public slots:
    void onDefaultExperimentPathChange(const QString& path);
    void onInsertStepBefore(int id, int type);
    void onInsertStepAfter(int id, int type);
    void onDeleteStep(int id);

    // Helper slots
private slots:
    void onExperimentChange();

private:
    void newWindowTitle();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString mDefaultPath;
    cExperimentFile mExperimentFile;
};

