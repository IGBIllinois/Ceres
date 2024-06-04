
#include "ExperimentDesignMdiChild.hpp"

#include "ExperimentSteps.hpp"

#include <QtWidgets>


cExperimentDesignMdiChild::cExperimentDesignMdiChild(QWidget *parent) : cExperimentDesignWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
}

void cExperimentDesignMdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
}

bool cExperimentDesignMdiChild::loadFile(const QString &fileName)
{
    return true;
}

bool cExperimentDesignMdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool cExperimentDesignMdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool cExperimentDesignMdiChild::saveFile(const QString &fileName)
{
    QString errorMessage;

/*
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("MDI"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
*/

    return true;
}

QString cExperimentDesignMdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void cExperimentDesignMdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void cExperimentDesignMdiChild::onExperimentChange()
{
    setWindowModified(mExperimentFile.isDirty());
}

bool cExperimentDesignMdiChild::maybeSave()
{
    if (!mExperimentFile.isDirty())
        return true;

    return false;
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
    default:
        return;
    }

    loadExperiment(mExperimentFile);
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
    default:
        return;
    }

    loadExperiment(mExperimentFile);
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
}


void cExperimentDesignMdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString cExperimentDesignMdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


