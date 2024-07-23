
#include "ExperimentDesignMdiChild.hpp"

#include "ExperimentSteps.hpp"
#include "ExperimentSteps_HySpex.hpp"

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

    mExperimentFile.clearSteps();

    loadExperiment(mExperimentFile);
}


void cExperimentDesignMdiChild::newFile(const cExperimentFile& file)
{
    auto filename = file.getFileName();
    if (filename.empty())
    {
        auto title = file.getExperimentName();
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

    mExperimentFile.clearSteps();

    mExperimentFile = file;

    loadExperiment(mExperimentFile);

    onExperimentChange();
}


void cExperimentDesignMdiChild::loadFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    mExperimentFile.clear();

    mExperimentFile.open(fileName.toStdString());

    loadExperiment(mExperimentFile);

    setCurrentFile(QFileInfo(fileName).canonicalFilePath());
}

void cExperimentDesignMdiChild::save()
{
    if (!mExperimentFile.isDirty())
        return;

    if (mExperimentFile.getFileName().empty())
    {
        saveAs();
    }
    else
    {
        mExperimentFile.save();
        setWindowModified(false);
    }
}

void cExperimentDesignMdiChild::saveAs()
{
    if (mExperimentFile.empty())
        return;

    QString defaultDirectory = mDefaultPath;

    if (getFileName().empty())
    {
        if (!mExperimentFile.getExperimentName().empty())
        {
            QFileInfo path;
            QString filename = QString::fromStdString(nStringUtils::safeFilename(mExperimentFile.getExperimentName()));

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

    mExperimentFile.save_as(fileName.toStdString());

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
    return QString::fromStdString(mExperimentFile.getFileName());
}

const std::string& cExperimentDesignMdiChild::getFileName() const
{
    return mExperimentFile.getFileName();
}

const std::string& cExperimentDesignMdiChild::getExperimentTitle() const
{
    return mExperimentFile.getExperimentName();
}

const cExperimentFile& cExperimentDesignMdiChild::getExperimentFile() const
{
    return mExperimentFile;
}

void cExperimentDesignMdiChild::setExperimentFile(const cExperimentFile& file)
{
    mExperimentFile = file;

    loadExperiment(mExperimentFile);

    if (mExperimentFile.isDirty())
        onExperimentChange();
}

const std::string& cExperimentDesignMdiChild::getLayoutName() const
{
    return mExperimentFile.getLayoutName();
}

void cExperimentDesignMdiChild::setLayoutName(const std::string& name)
{
    mExperimentFile.setLayoutName(name);
}

void cExperimentDesignMdiChild::editMetaInfo()
{
    cExperimentMetaInfoDlg dlg(mExperimentFile.getMetaData(), this);

    dlg.setExperimentTitle(mExperimentFile.getExperimentName());

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    mExperimentFile.setExperimentName(dlg.getExperimentTitle());

    onExperimentChange();
}

void cExperimentDesignMdiChild::editCtrlInfo()
{
    cExperimentCtrlInfoDlg dlg(mExperimentFile.getController(), this);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    mExperimentFile.setController(std::move(dlg.getControllerInfo()));
    onExperimentChange();
}

void cExperimentDesignMdiChild::editSensorInfo()
{
    auto sensors = mExperimentFile.getSensors();
    cExperimentSensorInfoDlg dlg(sensors, this);
    auto result = dlg.exec();

    if (result == QDialog::Accepted)
    {
        mExperimentFile.clearSensors();
        mExperimentFile.setSensors(dlg.getSensorInfo());
        onExperimentChange();
    }
}


void cExperimentDesignMdiChild::set_X_Position(int x_mm)
{
    if ((x_mm < 10000) || (x_mm > 190000))
        return;

    for (auto& step : mExperimentFile)
    {
        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

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

    for (auto& step : mExperimentFile)
    {
        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

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

    for (auto& step : mExperimentFile)
    {
        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

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

    for (auto& step : mExperimentFile)
    {
        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

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
    loadExperiment(mExperimentFile);
}

void cExperimentDesignMdiChild::closeEvent(QCloseEvent *event)
{
    if (mExperimentFile.isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText("The experiment file has been modified.");
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
            cExperimentDesignWidget::redrawPath(mExperimentFile);
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
    setWindowModified(mExperimentFile.isDirty());
}

void cExperimentDesignMdiChild::onInsertStepBefore(int id, int type)
{
    switch (type)
    {
    case eExperimentStep::delay:
    {
        std::unique_ptr<cExperimentStep_Delay> step = std::make_unique<cExperimentStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    case eExperimentStep::pause:
    {
        std::unique_ptr<cExperimentStep_Pause> step = std::make_unique<cExperimentStep_Pause>();
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    case eExperimentStep::movement:
    {
        std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    case eExperimentStep::hyspex_command:
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

        std::unique_ptr<cExperimentStep_HySpex_Command> step = std::make_unique<cExperimentStep_HySpex_Command>(camera_type, command);

        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    default:
        return;
    }

    loadExperiment(mExperimentFile);
    onExperimentChange();
}

void cExperimentDesignMdiChild::onInsertStepAfter(int id, int type)
{
    switch (type)
    {
    case eExperimentStep::delay:
    {
        auto step = std::make_unique<cExperimentStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    case eExperimentStep::pause:
    {
        auto step = std::make_unique<cExperimentStep_Pause>();
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    case eExperimentStep::movement:
    {
        auto step = std::make_unique<cExperimentStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    case eExperimentStep::hyspex_command:
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

        std::unique_ptr<cExperimentStep_HySpex_Command> step = std::make_unique<cExperimentStep_HySpex_Command>(camera_type, command);

        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    default:
        return;
    }

    loadExperiment(mExperimentFile);
    onExperimentChange();
}

void cExperimentDesignMdiChild::onDeleteStep(int id)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete the experiment step?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        if (mExperimentFile.removeStep(id))
            loadExperiment(mExperimentFile);
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


