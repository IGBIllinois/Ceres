
#include "ExperimentDesignItems.hpp"

#include <QMenu>
#include <QAction>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneContextMenuEvent>

#include <algorithm>


/********************************************************************
 *
 * Flow Chart Step: Flow Arrow
 *
 ********************************************************************/

cFlowArrow::cFlowArrow(QGraphicsItem* parent) : QGraphicsItem(parent)
{}

void cFlowArrow::setTopPoint(int x, int y)
{
	setTopPoint(QPoint(x, y));
}

void cFlowArrow::setTopPoint(QPoint p)
{
	mTop = p;
	mBottom.setX(p.x());
	mBottom.setY(p.y() + mScale * mLineLength);
}

QPoint cFlowArrow::getBottomPoint() const
{
	return mBottom;
}

void cFlowArrow::setScale(int scale)
{
	mScale = scale;
	update();
}

void cFlowArrow::setPen(const QPen& pen)
{
	mPen = pen;
	update();
}

void cFlowArrow::setBrush(const QBrush& brush)
{
	mBrush = brush;
	update();
}

void cFlowArrow::setAntialiased(bool antialiased)
{
	mAntialiased = antialiased;
	update();
}

QRectF cFlowArrow::boundingRect() const
{
	qreal width  = mScale * 2.5 * mArrowHeadSize;
	qreal height = mScale * mLineLength;
	qreal x = mTop.x() - width / 2;
	qreal y = mTop.y();
	return QRectF(x, y, width, height);
}

void cFlowArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setPen(mPen);

	mBrush.setStyle(Qt::SolidPattern);
	painter->setBrush(mBrush);

	if (mAntialiased)
		painter->setRenderHint(QPainter::Antialiasing, true);

//	QBrush brush = painter->brush();
//	brush.setStyle(Qt::SolidPattern);
//	painter->setBrush(brush);

	painter->drawLine(mTop, mBottom);

	int x = (mScale * mArrowHeadSize) / 2;
	int y = mBottom.y() - (mScale * mArrowHeadSize);
	QPointF arrowHead[3] = { QPointF(-x, y), QPointF(0, mBottom.y()), QPointF(x, y) };
	painter->drawPolygon(arrowHead, 3);

	painter->restore();
}

void cFlowArrow::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	QAction before("Insert Step Before...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&before);

	QAction after("Insert Step After...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&after);

	contextMenu.exec(event->screenPos());
};


/********************************************************************
 * 
 * Flow Chart Step: Terminal 
 * 
 ********************************************************************/

cTerminal::cTerminal(QGraphicsItem* parent) : QGraphicsItem(parent)
{
	QFontMetrics fm(mFont);

	auto b = fm.boundingRect(mText);
	mMinTextWidth  = b.width();
	mMinTextHeight = b.height();

	if (mBoxWidth < mMinTextWidth)
		mBoxWidth = 1.5 * mMinTextWidth;

	if (mBoxHeight < mMinTextHeight)
		mBoxHeight = 1.5 * mMinTextHeight;

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

cTerminal::cTerminal(const QString& text, QGraphicsItem* parent)
{
	setText(text);

	if (mBoxWidth < mMinTextWidth)
		mBoxWidth = 1.5 * mMinTextWidth;

	if (mBoxHeight < mMinTextHeight)
		mBoxHeight = 1.5 * mMinTextHeight;

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

const QString& cTerminal::text() const
{
	return mText;
}

void cTerminal::setText(const QString& text)
{
	mText = text;

	QFontMetrics fm(mFont);

	auto b = fm.boundingRect(mText);
	mMinTextWidth = b.width();
	mMinTextHeight = b.height();
}

void cTerminal::setTopPoint(int x, int y)
{
	setTopPoint(QPoint(x, y));
}

void cTerminal::setTopPoint(QPoint p)
{
	mTop = p;
	mBottom.setX(p.x());
	mBottom.setY(p.y() + mScale * mBoxHeight);
}

QPoint cTerminal::getBottomPoint() const
{
	return mBottom;
}

void cTerminal::setScale(int scale)
{
	mScale = scale;
	update();
}

void cTerminal::setPen(const QPen& pen)
{
	mPen = pen;
	update();
}

void cTerminal::setBrush(const QBrush& brush)
{
	mBrush = brush;
	update();
}

void cTerminal::setFont(const QFont& font)
{
	mFont = font;
	update();
}

void cTerminal::setAntialiased(bool antialiased)
{
	mAntialiased = antialiased;
	update();
}

QRectF cTerminal::boundingRect() const
{
	return QRectF(mTop.x() - (mScale * mBoxWidth / 2), mTop.y(), mScale * mBoxWidth, mScale * mBoxHeight);
}

void cTerminal::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setPen(mPen);

	mBrush.setStyle(Qt::NoBrush);
	painter->setBrush(mBrush);

	if (mAntialiased)
		painter->setRenderHint(QPainter::Antialiasing, true);

	QPoint p1 = mTop;
	p1.setX(mTop.x() - (mScale * mBoxWidth / 2));

	QPoint p2 = mTop;
	p2.setX(mTop.x() + (mScale * mBoxWidth / 2));

	painter->drawLine(p1, p2);

	int w = mScale * mBoxHeight;
	int h = mScale * mBoxHeight;

	painter->drawArc(p1.x() - w / 2, mTop.y(), w, h, 90 * 16, 180 * 16);
	painter->drawArc(p2.x() - w / 2, mTop.y(), w, h, 270 * 16, 180 * 16);

	p1 = mBottom;
	p1.setX(mBottom.x() - (mScale * mBoxWidth / 2));

	p2 = mBottom;
	p2.setX(mBottom.x() + (mScale * mBoxWidth / 2));

	painter->drawLine(p1, p2);

	painter->setFont(mFont);

	int x = -mMinTextWidth / 2;
	int y = ((mBottom.y() + mTop.y()) / 2) + (mMinTextHeight / 2);
	painter->drawText(x, y, mText);

	painter->restore();
}


cStartTerminal::cStartTerminal(QGraphicsItem* parent)
	: cTerminal("Start", parent)
{}

void cStartTerminal::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	QAction open("Open...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&open);

	contextMenu.exec(event->screenPos());
};

cEndTerminal::cEndTerminal(QGraphicsItem* parent)
	: cTerminal("End", parent)
{}

void cEndTerminal::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	QAction open("Open...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&open);

	contextMenu.exec(event->screenPos());
};


/********************************************************************
 *
 * Flow Chart Step: Process Step
 *
 ********************************************************************/

cProcessStep::cProcessStep(QGraphicsItem* parent) : QGraphicsItem(parent)
{
	QFontMetrics fm(mFont);

	auto b = fm.boundingRect("M");
	mMinTextWidth = b.width();
	mMinTextHeight = b.height();

	if (mBoxWidth < mMinTextWidth)
		mBoxWidth = 1.5 * mMinTextWidth;

	if (mBoxHeight < mMinTextHeight)
		mBoxHeight = 1.5 * mMinTextHeight;

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

cProcessStep::cProcessStep(const QString& text, QGraphicsItem* parent)
{
	setTitle(text);

	if (mBoxWidth < mMinTextWidth)
		mBoxWidth = 1.5 * mMinTextWidth;

	if (mBoxHeight < mMinTextHeight)
		mBoxHeight = 1.5 * mMinTextHeight;

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

const QString& cProcessStep::title() const
{
	return mTitle;
}

void cProcessStep::setTitle(const QString& text)
{
	mTitle = text;

	QFontMetrics fm(mFont);

	auto b = fm.boundingRect(mTitle);
	mMinTextWidth = b.width();
	mMinTextHeight = b.height();
}

void cProcessStep::setTopPoint(int x, int y)
{
	setTopPoint(QPoint(x, y));
}

void cProcessStep::setTopPoint(QPoint p)
{
	mTop = p;
	mBottom.setX(p.x());
	mBottom.setY(p.y() + mScale * mBoxHeight);
}

QPoint cProcessStep::getBottomPoint() const
{
	return mBottom;
}

void cProcessStep::setScale(int scale)
{
	mScale = scale;
	update();
}

void cProcessStep::setPen(const QPen& pen)
{
	mPen = pen;
	update();
}

void cProcessStep::setBrush(const QBrush& brush)
{
	mBrush = brush;
	update();
}

void cProcessStep::setFont(const QFont& font)
{
	mFont = font;
	update();
}

void cProcessStep::setAntialiased(bool antialiased)
{
	mAntialiased = antialiased;
	update();
}

QRectF cProcessStep::boundingRect() const
{
	qreal width = mScale * mBoxWidth;
	qreal height = mScale * mBoxHeight;
	qreal x = mTop.x() - (width / 2);
	qreal y = mTop.y();
	return QRectF(x, y, width, height);
}

void cProcessStep::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setPen(mPen);

	mBrush.setStyle(Qt::NoBrush);
	painter->setBrush(mBrush);

	if (mAntialiased)
		painter->setRenderHint(QPainter::Antialiasing, true);

	QPoint p1 = mTop;
	p1.setX(mTop.x() - (mScale * mBoxWidth / 2));

	painter->drawRoundRect(p1.x(), p1.y(), mScale * mBoxWidth, mScale * mBoxHeight, 10, 10);

	painter->setFont(mFont);

	int x = -mMinTextWidth / 2;
	int y = ((mBottom.y() + mTop.y()) / 2) + (mMinTextHeight / 2);
	painter->drawText(x, y, mTitle);

	painter->restore();
}

void cProcessStep::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	QAction edit("Edit...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&edit);

	contextMenu.addSeparator();

	QAction before("Insert Step Before...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&before);

	QAction after("Insert Step After...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&after);

	contextMenu.exec(event->screenPos());
}

