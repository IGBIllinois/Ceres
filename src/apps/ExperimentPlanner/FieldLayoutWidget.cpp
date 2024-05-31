
#include "FieldLayoutWidget.hpp"
#include "FieldLayoutDlg.hpp"

#include <QLayout>
#include <QContextMenuEvent>


cFieldLayoutWidget::cFieldLayoutWidget(QWidget* parent)
	: QWidget(parent)
{
}

void cFieldLayoutWidget::initialize()
{
    setMinimumWidth(400);

    mpScanArea = new cSpidercamScanArea(this);
    mpScanArea->hideDollyPosition();

    auto* mainlayout = new QVBoxLayout();
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpScanArea);
    mainlayout->addSpacing(10);

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

void cFieldLayoutWidget::clearRecordingPath()
{
    mpScanArea->clearRecordedPath();
    mpScanArea->repaint();
}

void cFieldLayoutWidget::drawRecordingPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm)
{
    mpScanArea->setRecording(true);
    mpScanArea->updateDollyPosition(x1_mm, y1_mm);
    mpScanArea->updateDollyPosition(x2_mm, y2_mm);
    mpScanArea->setRecording(false);
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

