
#include "ExperimentDesignWidget.hpp"
#include "ExperimentDesignItems.hpp"
#include "ExperimentFile.hpp"
#include "ExperimentSteps.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QPainter>
#include <QPainterPath>

#include <algorithm>


cExperimentDesignWidget::cExperimentDesignWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    mAntialiased = false;
    mTransformed = false;

    setScene(&mScene);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void cExperimentDesignWidget::clear()
{
    mScene.clear();
    show();
}

void cExperimentDesignWidget::loadExperiment(const cExperimentFile& experiment)
{
    mScene.clear();

    emit clearPaths();

    cTerminal* start = new cStartTerminal();
    mScene.addItem(start);

    cFlowArrow* arrow = new cFlowArrow(0);
    connect(arrow, &cFlowArrow::insertBefore, this, &cExperimentDesignWidget::insertBefore);
    arrow->setTopPoint(start->getBottomPoint());
    mScene.addItem(arrow);

    int x1_mm = 0;
    int y1_mm = 0;

    int id = 0;

    for (auto step : experiment)
    {
        auto item = step->graphicsItem(id);
        connect(item, &cBaseStep::insertBefore, this, &cExperimentDesignWidget::insertBefore);
        connect(item, &cBaseStep::insertAfter, this, &cExperimentDesignWidget::insertAfter);
        connect(item, &cBaseStep::deleteStep, this, &cExperimentDesignWidget::deleteStep);
        item->setTopPoint(arrow->getBottomPoint());
        mScene.addItem(item);

        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getX_mm().has_value() && movement->getY_mm().has_value())
            {
                if (movement->isRecording())
                {
                    int x2_mm = movement->getX_mm().value();
                    int y2_mm = movement->getY_mm().value();

                    emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);

                    x1_mm = x2_mm;
                    y1_mm = y2_mm;
                }
                else
                {
                    x1_mm = movement->getX_mm().value();
                    y1_mm = movement->getY_mm().value();
                }
            }
        }

        ++id;
        arrow = new cFlowArrow(id);
        connect(arrow, &cFlowArrow::insertBefore, this, &cExperimentDesignWidget::insertBefore);
        arrow->setTopPoint(item->getBottomPoint());
        mScene.addItem(arrow);
    }

    cTerminal* end = new cEndTerminal();
    end->setTopPoint(arrow->getBottomPoint());
    mScene.addItem(end);

    show();
}

void cExperimentDesignWidget::redrawPath(const cExperimentFile& experiment)
{
    emit clearPaths();

    int x1_mm = 0;
    int y1_mm = 0;

    int id = 0;

    for (auto step : experiment)
    {
        auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

        if (movement)
        {
            if (movement->getX_mm().has_value() && movement->getY_mm().has_value())
            {
                if (movement->isRecording())
                {
                    int x2_mm = movement->getX_mm().value();
                    int y2_mm = movement->getY_mm().value();

                    emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);

                    x1_mm = x2_mm;
                    y1_mm = y2_mm;
                }
                else
                {
                    x1_mm = movement->getX_mm().value();
                    y1_mm = movement->getY_mm().value();
                }
            }
        }
    }
}

QSize cExperimentDesignWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize cExperimentDesignWidget::sizeHint() const
{
    return QSize(400, 200);
}

void cExperimentDesignWidget::setPen(const QPen &pen)
{
    mPen = pen;
    update();
}

void cExperimentDesignWidget::setBrush(const QBrush &brush)
{
    mBrush = brush;
    update();
}

void cExperimentDesignWidget::setAntialiased(bool antialiased)
{
    mAntialiased = antialiased;
    update();
}

void cExperimentDesignWidget::setTransformed(bool transformed)
{
    mTransformed = transformed;
    update();
}



