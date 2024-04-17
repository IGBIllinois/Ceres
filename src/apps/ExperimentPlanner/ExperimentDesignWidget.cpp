
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

/*
    cTerminal* start = new cStartTerminal();
    mScene.addItem(start);

    cFlowArrow* arrow = new cFlowArrow();
    arrow->setTopPoint(start->getBottomPoint());
    mScene.addItem(arrow);

    cProcessStep* step = new cProcessStep();
    step->setTopPoint(arrow->getBottomPoint());
    mScene.addItem(step);

    arrow = new cFlowArrow();
    arrow->setTopPoint(step->getBottomPoint());
    mScene.addItem(arrow);

    cTerminal* end = new cEndTerminal();
    end->setTopPoint(arrow->getBottomPoint());
    mScene.addItem(end);

    show();
*/
}

void cExperimentDesignWidget::clear()
{
    mScene.clear();
    show();
}

void cExperimentDesignWidget::loadExperiment(const cExperimentFile& experiment)
{
    mScene.clear();

    cTerminal* start = new cStartTerminal();
    mScene.addItem(start);

    cFlowArrow* arrow = new cFlowArrow();
    arrow->setTopPoint(start->getBottomPoint());
    mScene.addItem(arrow);

    for (auto step : experiment)
    {
        auto item = step->graphicsItem();
        item->setTopPoint(arrow->getBottomPoint());
        mScene.addItem(item);

        arrow = new cFlowArrow();
        arrow->setTopPoint(item->getBottomPoint());
        mScene.addItem(arrow);
    }

    cTerminal* end = new cEndTerminal();
    end->setTopPoint(arrow->getBottomPoint());
    mScene.addItem(end);

    show();
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

#if 0
void cExperimentDesignWidget::paintEvent(QPaintEvent* /* event */)
{
    static const QPoint points[4] = {
        QPoint(10, 80),
        QPoint(20, 10),
        QPoint(80, 30),
        QPoint(90, 70)
    };

    QRect rect(10, 20, 80, 60);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QPainter painter(this);
    painter.setPen(mPen);
    painter.setBrush(mBrush);
    if (mAntialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    for (int x = 0; x < width(); x += 100)
    {
        for (int y = 0; y < height(); y += 100) 
        {
            painter.save();
            painter.translate(x, y);
 
			if (mTransformed) 
			{
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }


            painter.restore();
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
#endif


