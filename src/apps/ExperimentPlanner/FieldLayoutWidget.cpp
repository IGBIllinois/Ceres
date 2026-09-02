
#include "FieldLayoutWidget.hpp"
#include "FieldLayoutDlg.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QGroupBox>
#include <QContextMenuEvent>
#include <QStatusBar>


cFieldLayoutWidget::cFieldLayoutWidget(QWidget* parent)
	: QWidget(parent)
{
}

void cFieldLayoutWidget::initialize()
{
    setMinimumWidth(400);

    mpScanArea = new cSpidercamScanArea(this);
    mpScanArea->hideDollyPosition();

    QLabel* pX_Label = new QLabel();
    pX_Label->setText("X (m)");

    mpX_m = new QLineEdit();
    mpX_m->setReadOnly(true);

    QLabel* pY_Label = new QLabel();
    pY_Label->setText("Y (m)");

    mpY_m = new QLineEdit();
    mpY_m->setReadOnly(true);

    QLabel* pZ_Label = new QLabel();
    pZ_Label->setText("Z (m)");

    mpZ_m = new QLineEdit();
    mpZ_m->setReadOnly(true);

//    mpPosInfo = new QGroupBox(this);
    mpPosInfo = new QWidget(this);
    mpPosInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* statuslayout = new QHBoxLayout();
    statuslayout->addWidget(pX_Label);
    statuslayout->addWidget(mpX_m);
    statuslayout->addWidget(pY_Label);
    statuslayout->addWidget(mpY_m);
    statuslayout->addWidget(pZ_Label);
    statuslayout->addWidget(mpZ_m);

    mpPosInfo->setLayout(statuslayout);
    mpPosInfo->hide();

    mpExperimentStatus = new QStatusBar();
    mpExperimentStatus->setHidden(true);
    mpExperimentStatus->setSizeGripEnabled(false);
    mpExperimentStatus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mpExperimentStatus->setStyleSheet("background-color: rgb(8, 255, 8);");

    auto* mainlayout = new QVBoxLayout();
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpScanArea);
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpExperimentStatus);
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpPosInfo);

    setLayout(mainlayout);
}

bool cFieldLayoutWidget::isDirty() const
{
    return mDirty;
}

void cFieldLayoutWidget::setBounds(double minX_mm, double maxX_mm, double minY_mm, double maxY_mm)
{
    if (maxX_mm < minX_mm) std::swap(minX_mm, maxX_mm);
    if (maxY_mm < minY_mm) std::swap(minY_mm, maxY_mm);

    mpScanArea->updateBounds(minX_mm, maxX_mm, minY_mm, maxY_mm);
}

void cFieldLayoutWidget::addLayout(const cSpidercamScanArea::experimentLayout_t& layout)
{
    mpScanArea->addLayout(layout);
    mDirty = true;
}

void cFieldLayoutWidget::replaceLayout(const cSpidercamScanArea::experimentLayout_t& original_layout, const cSpidercamScanArea::experimentLayout_t& new_layout)
{
    mpScanArea->replaceLayout(original_layout, new_layout);
    mDirty |= new_layout != original_layout;
}

cSpidercamScanArea::experimentLayout_t cFieldLayoutWidget::findLayout(const QString& label)
{
    const std::vector<cSpidercamScanArea::experimentLayout_t>& layouts = mpScanArea->getLayouts();

    for (auto layout : layouts)
    {
        if (layout.caption.label == label)
        {
            return layout;
        }
    }

   return cSpidercamScanArea::experimentLayout_t();
}

const std::vector<cSpidercamScanArea::experimentLayout_t>& cFieldLayoutWidget::getLayouts() const
{
    return mpScanArea->getLayouts();
}

void cFieldLayoutWidget::onConnectToSpidercam()
{
    mpPosInfo->show();
    mpScanArea->showDollyPosition();
    mpScanArea->repaint();
}

void cFieldLayoutWidget::onDisconnectFromSpidercam()
{
    mpPosInfo->hide();
    mpScanArea->hideDollyPosition();
    mpScanArea->repaint();
}

void cFieldLayoutWidget::updateLimits(spidercam::sWorkingDimensions limits)
{
    auto mMinX_mm = limits.minX_mm;
    auto mMaxX_mm = limits.maxX_mm;
    auto mMinY_mm = limits.minY_mm;
    auto mMaxY_mm = limits.maxY_mm;
//    mMinHeight_mm = limits.minHeight_mm;
//    mMaxHeight_mm = limits.maxHeight_mm;

    mpScanArea->updateBounds(mMinX_mm, mMaxX_mm, mMinY_mm, mMaxY_mm);
}

void cFieldLayoutWidget::updatePosition(spidercam::sPosition_1_t pos)
{
    mpScanArea->updateDollyPosition(pos.X_mm, pos.Y_mm);

    mX_mm = pos.X_mm;
    mY_mm = pos.Y_mm;
    mZ_mm = pos.Z_mm;

    mpX_m->setText(QString::number(pos.X_mm * nConstants::MM_TO_M, 'f', 3));
    mpY_m->setText(QString::number(pos.Y_mm * nConstants::MM_TO_M, 'f', 3));
    mpZ_m->setText(QString::number(pos.height_mm * nConstants::MM_TO_M, 'f', 3));
}

void cFieldLayoutWidget::updateRecordingState(bool recording)
{
    mpScanArea->setRecording(recording);
}

void cFieldLayoutWidget::clearRecordingPath()
{
    mpScanArea->clearRecordedPath();
    mpScanArea->repaint();
}

void cFieldLayoutWidget::drawRecordingPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm)
{
    int x_mm = -1;
    int y_mm = -1;

    if (mpScanArea->isDollyPositionVisible())
    {
        auto pos = mpScanArea->getDollyPosition();
        x_mm = pos.x;
        y_mm = pos.y;
    }

    mpScanArea->setRecording(true);
    mpScanArea->updateDollyPosition(x1_mm, y1_mm);
    mpScanArea->updateDollyPosition(x2_mm, y2_mm);
    mpScanArea->setRecording(false);

    if ((x_mm > 0) && (y_mm > 0))
    {
        mpScanArea->updateDollyPosition(x_mm, y_mm);
    }
}

void cFieldLayoutWidget::refresh()
{
//    if (mLayoutFilename.empty()) return;

//    mpScanArea->loadLayout(mLayoutFilename);
}

void cFieldLayoutWidget::experimentStateChanging(experiment::eState state)
{
    using namespace experiment;

    switch (state)
    {
    case eState::LOADED:
        mpExperimentStatus->setHidden(false);
        break;
    case eState::RUNNING:
        mpExperimentStatus->setStyleSheet("background-color: rgb(8, 255, 8);");
        break;
    case eState::PAUSED:
        mpExperimentStatus->setStyleSheet("background-color: rgb(255, 191, 0);");
        break;
    case eState::COMPLETED:
    case eState::TERMINATED:
        mpExperimentStatus->clearMessage();
        mpExperimentStatus->setHidden(true);
        break;
    case eState::EXP_ERROR:
        mpExperimentStatus->setStyleSheet("background-color: rgb(235, 33, 46);");
        break;
    }
}

void cFieldLayoutWidget::experimentStatusUpdating(QString msg)
{
    mpExperimentStatus->showMessage(msg);
}

void cFieldLayoutWidget::load(const QString& layout_filename)
{
    if (layout_filename.isEmpty())
        return;

    mpScanArea->loadLayout(layout_filename.toStdString());
}

void cFieldLayoutWidget::save(const QString& layout_filename)
{
    if (layout_filename.isEmpty())
        return;

    mpScanArea->saveLayout(layout_filename.toStdString());

    mDirty = false;
}

void cFieldLayoutWidget::contextMenuEvent(QContextMenuEvent* event)
{
    auto x = event->x();
    auto y = event->y();

    auto& layouts = mpScanArea->getLayouts();

    for (auto& layout : layouts)
    {
        std::string label = layout.caption.label.toStdString();

        if (layout.pos.contains(x, y))
        {
            auto original = layout;

            cFieldLayoutDlg dlg(this);

            dlg.setDefaults(original);

            auto result = dlg.exec();

            if (result == QDialog::Rejected)
                return;

            auto new_layout = dlg.getLayout();

            if (original != new_layout)
            {
                mpScanArea->replaceLayout(original, new_layout);
                mDirty = true;
            }

            break;
        }
    }
}

