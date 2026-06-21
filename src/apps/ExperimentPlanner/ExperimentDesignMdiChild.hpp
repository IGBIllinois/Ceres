
#pragma once

#include "ExperimentDesignWidget.hpp"

#include "ExperimentFile.hpp"

#include <string>


class cExperimentDesignMdiChild : public cExperimentDesignWidget
{
    Q_OBJECT

public:
    cExperimentDesignMdiChild(QWidget *parent);

    void newFile(eExperimentType exp_type);
    void newFile(const cExperimentFile& file);
    void loadFile(const QString &fileName);
    void save();
    void saveAs();

    QString userFriendlyCurrentFile();
    QString currentFile();

    const std::string& getFileName() const;

    const std::string& getMeasurementTitle() const;

    const cExperimentFile& getMeasurementFile() const;
    void setMeasurementFile(const cExperimentFile& file);

    void editMetaInfo();
    void editCtrlInfo();
    void editSensorInfo();

    std::pair<int, int> x_mm() const;
    std::pair<int, int> y_mm() const;
    std::pair<int, int> z_mm() const;

    void set_X_Position(int x_mm);
    void set_Y_Position(int y_mm);
    void set_Z_Position(int z_mm);

    void shiftPositions(int x_mm, int y_mm, int z_mm);

    void reloadPath();

    cExperimentFile::iterator  begin();
    cExperimentFile::iterator  end();

    cExperimentFile::const_iterator	begin() const;
    cExperimentFile::const_iterator	end() const;

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
    cExperimentFile mMeasurementFile;
};

