
#include "ExperimentDesignMdiChild.hpp"

#include "MeasurementSteps.hpp"
#include "MeasurementSteps_HySpex.hpp"
#include "MeasurementSteps_Ssnx.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "HySpexCommandDlg.hpp"

#include "StringUtils.hpp"

#include <QtWidgets>
#include <QWindowStateChangeEvent>

cExperimentDesignMdiChild::cExperimentDesignMdiChild(QWidget *parent) : cExperimentDesignWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    connect(this, &cExperimentDesignWidget::insertBefore, this, &cExperimentDesignMdiChild::onInsertStepBefore);
    connect(this, &cExperimentDesignWidget::insertAfter, this, &cExperimentDesignMdiChild::onInsertStepAfter);
    connect(this, &cExperimentDesignWidget::deleteStep, this, &cExperimentDesignMdiChild::onDeleteStep);
}

void cExperimentDesignMdiChild::newWindowTitle()
{
    static int sequenceNumber = 1;

    QString curFile = tr("experiment%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
}

void cExperimentDesignMdiChild::newFile()
{
    newWindowTitle();

    mMeasurementFile.clearSteps();

    loadExperiment(mMeasurementFile);
}


void cExperimentDesignMdiChild::newFile(const cExperimentFile& file)
{
    auto filename = file.getFileName();
    if (filename.empty())
    {
        auto title = file.getMeasurementName();
        if (title.empty())
        {
            newWindowTitle();
        }
        else
        {
            QString curFile = QString::fromStdString(title);
            setWindowTitle(curFile + "[*]");
        }

    }
    else
    {
        QString curFile = QString::fromStdString(filename);
        setWindowTitle(curFile + "[*]");
    }

    mMeasurementFile.clearSteps();

    mMeasurementFile = file;

    loadExperiment(mMeasurementFile);

    onExperimentChange();
}


void cExperimentDesignMdiChild::loadFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    mMeasurementFile.clear();

    mMeasurementFile.open(fileName.toStdString());

    loadExperiment(mMeasurementFile);

    setCurrentFile(QFileInfo(fileName).canonicalFilePath());
}

void cExperimentDesignMdiChild::save()
{
    if (!mMeasurementFile.isDirty())
        return;

    if (mMeasurementFile.getFileName().empty())
    {
        saveAs();
    }
    else
    {
        mMeasurementFile.save();
        setWindowModified(false);
    }
}

void cExperimentDesignMdiChild::saveAs()
{
    if (mMeasurementFile.empty())
        return;

    QString defaultDirectory = mDefaultPath;

    if (getFileName().empty())
    {
        if (!mMeasurementFile.getMeasurementName().empty())
        {
            QFileInfo path;
            QString filename = QString::fromStdString(nStringUtils::safeFilename(mMeasurementFile.getMeasurementName()));

            path.setFile(defaultDirectory, filename);

            defaultDirectory = path.absoluteFilePath();
        }
        else if (!mMeasurementFile.getExperimentName().empty())
        {
            QFileInfo path;
            QString filename = QString::fromStdString(nStringUtils::safeFilename(mMeasurementFile.getExperimentName()));

            path.setFile(defaultDirectory, filename);

            defaultDirectory = path.absoluteFilePath();
        }
    }
    else
        defaultDirectory = QString::fromStdString(getFileName());

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Experiment File"), defaultDirectory,
        "Experiment Files (*.json)");

    if (fileName.isEmpty())
        return;

    mMeasurementFile.save_as(fileName.toStdString());

    setCurrentFile(QFileInfo(fileName).canonicalFilePath());

    emit experimentListNeedsUpdate();

    return;
}

QString cExperimentDesignMdiChild::userFriendlyCurrentFile()
{
    return strippedName(currentFile());
}

QString cExperimentDesignMdiChild::currentFile()
{
    return QString::fromStdString(mMeasurementFile.getFileName());
}

const std::string& cExperimentDesignMdiChild::getFileName() const
{
    return mMeasurementFile.getFileName();
}

const std::string& cExperimentDesignMdiChild::getMeasurementTitle() const
{
    if (mMeasurementFile.getMeasurementName().empty())
        return mMeasurementFile.getExperimentName();

    return mMeasurementFile.getExperimentName();
}

const cExperimentFile& cExperimentDesignMdiChild::getMeasurementFile() const
{
    return mMeasurementFile;
}

void cExperimentDesignMdiChild::setMeasurementFile(const cExperimentFile& file)
{
    mMeasurementFile = file;

    loadExperiment(mMeasurementFile);

    if (mMeasurementFile.isDirty())
        onExperimentChange();
}

const std::string& cExperimentDesignMdiChild::getLayoutName() const
{
    return mMeasurementFile.getLayoutName();
}

void cExperimentDesignMdiChild::setLayoutName(const std::string& name)
{
    mMeasurementFile.setLayoutName(name);
}

void cExperimentDesignMdiChild::editMetaInfo()
{
    cExperimentMetaInfoDlg dlg(mMeasurementFile.getMetaData(), this);

    dlg.setMeasurementTitle(mMeasurementFile.getMeasurementName());
    dlg.setExperimentTitle(mMeasurementFile.getExperimentName());

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    mMeasurementFile.setExperimentName(dlg.getExperimentTitle());

    onExperimentChange();
}

void cExperimentDesignMdiChild::editCtrlInfo()
{
    cExperimentCtrlInfoDlg dlg(mMeasurementFile.getController(), this);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    mMeasurementFile.setController(std::move(dlg.getControllerInfo()));
    onExperimentChange();
}

void cExperimentDesignMdiChild::editSensorInfo()
{
    auto sensors = mMeasurementFile.getSensors();
    cExperimentSensorInfoDlg dlg(sensors, this);
    auto result = dlg.exec();

    if (result == QDialog::Accepted)
    {
        mMeasurementFile.clearSensors();
        mMeasurementFile.setSensors(dlg.getSensorInfo());
        onExperimentChange();
    }
}


std::pair<int, int> cExperimentDesignMdiChild::x_mm() const
{
    int start_x = -1;
    int end_x = -1;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getX_mm().has_value())
            {
                if (start_x == -1)
                    start_x = movement->getX_mm().value();
                else
                    end_x = movement->getX_mm().value();
            }
        }
    }

    return { start_x, end_x };
}

std::pair<int, int> cExperimentDesignMdiChild::y_mm() const
{
    int start_y = -1;
    int end_y = -1;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getY_mm().has_value())
            {
                if (start_y == -1)
                    start_y = movement->getY_mm().value();
                else
                    end_y = movement->getY_mm().value();
            }
        }
    }

    return { start_y, end_y };
}

std::pair<int, int> cExperimentDesignMdiChild::z_mm() const
{
    int start_z = -1;
    int end_z = -1;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getZ_mm().has_value())
            {
                if (start_z == -1)
                    start_z = movement->getZ_mm().value();
                else
                    end_z = movement->getZ_mm().value();
            }
        }
    }

    return { start_z, end_z };
}


void cExperimentDesignMdiChild::set_X_Position(int x_mm)
{
    if ((x_mm < 10000) || (x_mm > 190000))
        return;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getX_mm().has_value())
            {
                movement->setX_mm(x_mm);
            }
        }
    }

    reloadPath();

    onExperimentChange();
}

void cExperimentDesignMdiChild::set_Y_Position(int y_mm)
{
    if ((y_mm < 10000) || (y_mm > 190000))
        return;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getY_mm().has_value())
            {
                movement->setY_mm(y_mm);
            }
        }
    }

    reloadPath();

    onExperimentChange();
}

void cExperimentDesignMdiChild::set_Z_Position(int z_mm)
{
    if ((z_mm < 1000) || (z_mm > 10000))
        return;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getZ_mm().has_value())
            {
                movement->setZ_mm(z_mm);
            }
        }
    }

    onExperimentChange();
}

void cExperimentDesignMdiChild::shiftPositions(int x_mm, int y_mm, int z_mm)
{
    if ((x_mm == 0) && (y_mm == 0) && (z_mm == 0))
        return;

    for (auto& step : mMeasurementFile)
    {
        auto movement = dynamic_cast<cMeasurementStep_Movement*>(step.get());

        if (movement)
        {
            if ((x_mm != 0) && movement->getX_mm().has_value())
            {
                int new_x_mm = movement->getX_mm().value() + x_mm;
                movement->setX_mm(new_x_mm);
            }

            if ((y_mm != 0) && movement->getY_mm().has_value())
            {
                int new_y_mm = movement->getY_mm().value() + y_mm;
                movement->setY_mm(new_y_mm);
            }

            if ((z_mm != 0) && movement->getZ_mm().has_value())
            {
                int new_z_mm = movement->getZ_mm().value() + z_mm;

                if (new_z_mm < 1000)
                    new_z_mm = 1000;

                if (new_z_mm > 10000)
                    new_z_mm = 10000;

                movement->setZ_mm(new_z_mm);
            }
        }
    }

    onExperimentChange();
}

void cExperimentDesignMdiChild::reloadPath()
{
    loadExperiment(mMeasurementFile);
}

void cExperimentDesignMdiChild::closeEvent(QCloseEvent *event)
{
    if (mMeasurementFile.isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText("The measurement file has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            save();
        }
        else if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    if (hasFocus())
    {
        emit clearPaths();
    }

    event->accept();
}

void cExperimentDesignMdiChild::focusInEvent(QFocusEvent* event)
{
    if (event)
    {
        if (event->gotFocus())
        {
            cExperimentDesignWidget::redrawPath(mMeasurementFile);
        }
    }

    cExperimentDesignWidget::focusInEvent(event);
}

void cExperimentDesignMdiChild::onDefaultExperimentPathChange(const QString& path)
{
    mDefaultPath = path;
}

void cExperimentDesignMdiChild::onExperimentChange()
{
    setWindowModified(mMeasurementFile.isDirty());
}

void cExperimentDesignMdiChild::onInsertStepBefore(int id, int type)
{
    switch (type)
    {
    case eMeasurementStep::delay:
    {
        std::unique_ptr<cMeasurementStep_Delay> step = std::make_unique<cMeasurementStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mMeasurementFile.insertBefore(id, std::move(step));
        break;
    }
    case eMeasurementStep::pause:
    {
        std::unique_ptr<cMeasurementStep_Pause> step = std::make_unique<cMeasurementStep_Pause>();
        mMeasurementFile.insertBefore(id, std::move(step));
        break;
    }
    case eMeasurementStep::movement:
    {
        std::unique_ptr<cMeasurementStep_Movement> step = std::make_unique<cMeasurementStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mMeasurementFile.insertBefore(id, std::move(step));
        break;
    }
    case eMeasurementStep::hyspex_command:
    {
        cHySpexCommandDlg dlg(this);

        auto result = dlg.exec();

        if (result == QDialog::Rejected)
            return;

        std::string camera_type;

        switch (dlg.getCameraType())
        {
        case cHySpexCommandDlg::eCameraType::eVNIR_3000N:
            camera_type = "VNIR-3000N";
            break;
        case cHySpexCommandDlg::eCameraType::eSWIR_384:
            camera_type = "SWIR-384";
            break;
        default:
            return;
        }


        std::string command;

        switch (dlg.getCommand())
        {
        case cHySpexCommandDlg::eCommandType::eOPEN_SHUTTER:
            command = "open shutter";
            break;
        case cHySpexCommandDlg::eCommandType::eCLOSE_SHUTTER:
            command = "close shutter";
            break;
        case cHySpexCommandDlg::eCommandType::eBACKGROUND:
            command = "background";
            break;
        default:
            return;
        }

        std::unique_ptr<cMeasurementStep_HySpex_Command> step = std::make_unique<cMeasurementStep_HySpex_Command>(camera_type, command);

        mMeasurementFile.insertBefore(id, std::move(step));
        break;
    }
    case eMeasurementStep::reference_point:
    {
        std::unique_ptr<cMeasurementStep_ReferencePoint> step = std::make_unique<cMeasurementStep_ReferencePoint>();
        if (!step->onEdit())
        {
            return;
        }
        mMeasurementFile.insertBefore(id, std::move(step));
        break;
    }
    default:
        return;
    }

    loadExperiment(mMeasurementFile);
    onExperimentChange();
}

void cExperimentDesignMdiChild::onInsertStepAfter(int id, int type)
{
    switch (type)
    {
    case eMeasurementStep::delay:
    {
        auto step = std::make_unique<cMeasurementStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mMeasurementFile.insertAfter(id, std::move(step));
        break;
    }
    case eMeasurementStep::pause:
    {
        auto step = std::make_unique<cMeasurementStep_Pause>();
        mMeasurementFile.insertAfter(id, std::move(step));
        break;
    }
    case eMeasurementStep::movement:
    {
        auto step = std::make_unique<cMeasurementStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mMeasurementFile.insertAfter(id, std::move(step));
        break;
    }
    case eMeasurementStep::hyspex_command:
    {
        cHySpexCommandDlg dlg(this);

        auto result = dlg.exec();

        if (result == QDialog::Rejected)
            return;

        std::string camera_type;

        switch (dlg.getCameraType())
        {
        case cHySpexCommandDlg::eCameraType::eVNIR_3000N:
            camera_type = "VNIR-3000N";
            break;
        case cHySpexCommandDlg::eCameraType::eSWIR_384:
            camera_type = "SWIR-384";
            break;
        default:
            return;
        }


        std::string command;

        switch (dlg.getCommand())
        {
        case cHySpexCommandDlg::eCommandType::eOPEN_SHUTTER:
            command = "open shutter";
            break;
        case cHySpexCommandDlg::eCommandType::eCLOSE_SHUTTER:
            command = "close shutter";
            break;
        case cHySpexCommandDlg::eCommandType::eBACKGROUND:
            command = "background";
            break;
        default:
            return;
        }

        std::unique_ptr<cMeasurementStep_HySpex_Command> step = std::make_unique<cMeasurementStep_HySpex_Command>(camera_type, command);

        mMeasurementFile.insertAfter(id, std::move(step));
        break;
    }
    default:
        return;
    }

    loadExperiment(mMeasurementFile);
    onExperimentChange();
}

void cExperimentDesignMdiChild::onDeleteStep(int id)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete the measurement step?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        if (mMeasurementFile.removeStep(id))
            loadExperiment(mMeasurementFile);
    }
    onExperimentChange();
}

void cExperimentDesignMdiChild::setCurrentFile(const QString &fileName)
{
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString cExperimentDesignMdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


