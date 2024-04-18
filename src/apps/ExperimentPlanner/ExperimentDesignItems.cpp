
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


cConnectedItem::cConnectedItem(QGraphicsItem* parent) : QGraphicsItem(parent)
{}

void cConnectedItem::setTopPoint(int x, int y)
{
	setTopPoint(QPoint(x, y));
}


/********************************************************************
 *
 * Flow Chart Step: Flow Arrow
 *
 ********************************************************************/

cFlowArrow::cFlowArrow(QGraphicsItem* parent) : cConnectedItem(parent)
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

	QAction step("Insert Step...");
	//	connect(&open, &QAction::triggered, this, &cExperimentManager::openExperiment);
	contextMenu.addAction(&step);

	contextMenu.exec(event->screenPos());
};


/********************************************************************
 * 
 * Flow Chart Step: Terminal 
 * 
 ********************************************************************/

cTerminal::cTerminal(QGraphicsItem* parent) : cConnectedItem(parent)
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

cTerminal::cTerminal(const QString& text, QGraphicsItem* parent) : cConnectedItem(parent)
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
	int y = ((mBottom.y() + mTop.y()) / 2) + (mMinTextHeight / 2) - 2;
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

cProcessStep::cProcessStep(QGraphicsItem* parent) : cConnectedItem(parent)
{
	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

cProcessStep::cProcessStep(const QString& text, QGraphicsItem* parent)
{
	setTitle(text);

	recomputeBoxSize();

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

const QString& cProcessStep::title() const
{
	return mTitle;
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

void cProcessStep::setTitle(const QString& title)
{
	mTitle = title;
	recomputeBoxSize();
	update();
}

void cProcessStep::setSubHeading1(const QString& heading)
{
	mSubHeading1 = heading;
	recomputeBoxSize();
	update();
}

void cProcessStep::setSubHeading2(const QString& heading)
{
	mSubHeading2 = heading;
	recomputeBoxSize();
	update();
}

void cProcessStep::setSubHeading3(const QString& heading)
{
	mSubHeading3 = heading;
	recomputeBoxSize();
	update();
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

void cProcessStep::recomputeBoxSize()
{
	QFontMetrics fm(mFont);

	auto title = fm.boundingRect(mTitle);
	auto heading1 = fm.boundingRect(mSubHeading1);
	auto heading2 = fm.boundingRect(mSubHeading2);
	auto heading3 = fm.boundingRect(mSubHeading3);

	int width = title.width() + 10;
	if (width < (heading1.width() + 10))
		width = heading1.width() + 10;
	if (width < (heading2.width() + 10))
		width = heading2.width() + 10;
	if (width < (heading3.width() + 10))
		width = heading3.width() + 10;

	if (mBoxWidth < width)
		mBoxWidth = 1.5 * width;

	int height = 5;	// <<-- Force space at top
	height += title.height();

	if (!mSubHeading1.isEmpty())
	{
		height += 5;	// <<-- Force space between title and sub heading 1
		height += heading1.height();
	}

	if (!mSubHeading2.isEmpty())
	{
		height += 5;	// <<-- Force space between sub heading 1 and sub heading 2
		height += heading2.height();
	}

	if (!mSubHeading3.isEmpty())
	{
		height += 5;	// <<-- Force space between sub heading 2 and sub heading 3
		height += heading3.height();
	}

	height += 5;	// <<-- Force space at bottom

	if (mBoxHeight < height)
		mBoxHeight = 1.25 * height;
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

	QFontMetrics fm(mFont);

	auto title = fm.boundingRect(mTitle);
	auto heading1 = fm.boundingRect(mSubHeading1);
	auto heading2 = fm.boundingRect(mSubHeading2);
	auto heading3 = fm.boundingRect(mSubHeading3);

	int cy = (mBottom.y() + mTop.y()) / 2;

	if (!mSubHeading3.isEmpty())
	{
		int x = -title.width() / 2;
		int y = cy - heading1.height() - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy - 2;
		painter->drawText(x, y, mSubHeading1);

		x = -heading2.width() / 2;
		y = cy + heading2.height() + 2;
		painter->drawText(x, y, mSubHeading2);

		x = -heading3.width() / 2;
		y = cy + heading2.height() + heading3.height() + 6;
		painter->drawText(x, y, mSubHeading3);
	}
	else if (!mSubHeading2.isEmpty())
	{
		int dy = heading1.height() / 2;

		int x = -title.width() / 2;
		int y = cy - dy - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy + dy;
		painter->drawText(x, y, mSubHeading1);

		x = -heading2.width() / 2;
		y = cy + dy + heading2.height() + 2;
		painter->drawText(x, y, mSubHeading2);
	}
	else if (!mSubHeading1.isEmpty())
	{
		int x = -title.width() / 2;
		int y = cy - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy + heading1.height();
		painter->drawText(x, y, mSubHeading1);
	}
	else
	{
		int x = -title.width() / 2;
		int y = cy + (title.height() / 2) - 2;
		painter->drawText(x, y, mTitle);
	}

	painter->restore();
}

void cProcessStep::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	QAction edit("Edit...");
	connect(&edit, &QAction::triggered, this, &cProcessStep::editStep);
	contextMenu.addAction(&edit);

	contextMenu.addSeparator();

	QAction before("Insert Step Before...");
	//	connect(&open, &QAction::triggered, this, &cProcessStep::openExperiment);
	contextMenu.addAction(&before);

	QAction after("Insert Step After...");
	//	connect(&open, &QAction::triggered, this, &cProcessStep::openExperiment);
	contextMenu.addAction(&after);

	contextMenu.exec(event->screenPos());
}


/********************************************************************
 *
 * Flow Chart Step: Input/Output Step
 *
 ********************************************************************/

cIoStep::cIoStep(QGraphicsItem* parent) : cConnectedItem(parent)
{
	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

cIoStep::cIoStep(const QString& text, QGraphicsItem* parent)
{
	setTitle(text);

	recomputeBoxSize();

	mTop.setY(-mScale * mBoxHeight / 2);
	mBottom.setY(mScale * mBoxHeight / 2);
}

const QString& cIoStep::title() const
{
	return mTitle;
}

void cIoStep::setTopPoint(int x, int y)
{
	setTopPoint(QPoint(x, y));
}

void cIoStep::setTopPoint(QPoint p)
{
	mTop = p;
	mBottom.setX(p.x());
	mBottom.setY(p.y() + mScale * mBoxHeight);
}

QPoint cIoStep::getBottomPoint() const
{
	return mBottom;
}

bool cIoStep::readOnly() const
{
	return !mAllowEdit;
}

void cIoStep::setReadOnly(bool read_only)
{
	mAllowEdit = !read_only;
}

void cIoStep::setTitle(const QString& title)
{
	mTitle = title;
	recomputeBoxSize();
	update();
}

void cIoStep::setSubHeading1(const QString& heading)
{
	mSubHeading1 = heading;
	recomputeBoxSize();
	update();
}

void cIoStep::setSubHeading2(const QString& heading)
{
	mSubHeading2 = heading;
	recomputeBoxSize();
	update();
}

void cIoStep::setSubHeading3(const QString& heading)
{
	mSubHeading3 = heading;
	recomputeBoxSize();
	update();
}

void cIoStep::setScale(int scale)
{
	mScale = scale;
	update();
}

void cIoStep::setPen(const QPen& pen)
{
	mPen = pen;
	update();
}

void cIoStep::setBrush(const QBrush& brush)
{
	mBrush = brush;
	update();
}

void cIoStep::setFont(const QFont& font)
{
	mFont = font;
	update();
}

void cIoStep::setAntialiased(bool antialiased)
{
	mAntialiased = antialiased;
	update();
}

void cIoStep::recomputeBoxSize()
{
	QFontMetrics fm(mFont);

	auto title = fm.boundingRect(mTitle);
	auto heading1 = fm.boundingRect(mSubHeading1);
	auto heading2 = fm.boundingRect(mSubHeading2);
	auto heading3 = fm.boundingRect(mSubHeading3);

	int width = title.width() + 10;
	if (width < (heading1.width() + 10))
		width = heading1.width() + 10;
	if (width < (heading2.width() + 10))
		width = heading2.width() + 10;
	if (width < (heading3.width() + 10))
		width = heading3.width() + 10;

	if (mBoxWidth < width)
		mBoxWidth = 1.5 * width;

	int height = 5;	// <<-- Force space at top
	height += title.height();

	if (!mSubHeading1.isEmpty())
	{
		height += 5;	// <<-- Force space between title and sub heading 1
		height += heading1.height();
	}

	if (!mSubHeading2.isEmpty())
	{
		height += 5;	// <<-- Force space between sub heading 1 and sub heading 2
		height += heading2.height();
	}

	if (!mSubHeading3.isEmpty())
	{
		height += 5;	// <<-- Force space between sub heading 2 and sub heading 3
		height += heading3.height();
	}

	height += 5;	// <<-- Force space at bottom

	if (mBoxHeight < height)
		mBoxHeight = 1.25 * height;

	mShift = 0.267949192 * (mBoxHeight / 2);  // <<-- The 0.267949192 is tan of 15 degrees
}

QRectF cIoStep::boundingRect() const
{
	qreal width = mScale * mBoxWidth + 2*mShift;
	qreal height = mScale * mBoxHeight;
	qreal x = mTop.x() - (width / 2) - mShift;
	qreal y = mTop.y();
	return QRectF(x, y, width, height);
}

void cIoStep::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setPen(mPen);

	mBrush.setStyle(Qt::NoBrush);
	painter->setBrush(mBrush);

	if (mAntialiased)
		painter->setRenderHint(QPainter::Antialiasing, true);

	QPoint points[4];

	QPoint p = mTop;
	p.setX(mTop.x() - (mScale * mBoxWidth / 2) + mShift);
	points[0] = p;

	p.setX(mTop.x() + (mScale * mBoxWidth / 2) + mShift);
	points[1] = p;

	p = mBottom;
	p.setX(mBottom.x() + (mScale * mBoxWidth / 2) - mShift);
	points[2] = p;

	p.setX(mBottom.x() - (mScale * mBoxWidth / 2) - mShift);
	points[3] = p;

	painter->drawPolygon(points, 4);

	painter->setFont(mFont);

	QFontMetrics fm(mFont);

	auto title = fm.boundingRect(mTitle);
	auto heading1 = fm.boundingRect(mSubHeading1);
	auto heading2 = fm.boundingRect(mSubHeading2);
	auto heading3 = fm.boundingRect(mSubHeading3);

	int cy = (mBottom.y() + mTop.y()) / 2;

	if (!mSubHeading3.isEmpty())
	{
		int x = -title.width() / 2;
		int y = cy - heading1.height() - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy - 2;
		painter->drawText(x, y, mSubHeading1);

		x = -heading2.width() / 2;
		y = cy + heading2.height() + 2;
		painter->drawText(x, y, mSubHeading2);

		x = -heading3.width() / 2;
		y = cy + heading2.height() + heading3.height() + 6;
		painter->drawText(x, y, mSubHeading3);
	}
	else if (!mSubHeading2.isEmpty())
	{
		int dy = heading1.height() / 2;

		int x = -title.width() / 2;
		int y = cy - dy - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy + dy;
		painter->drawText(x, y, mSubHeading1);

		x = -heading2.width() / 2;
		y = cy + dy + heading2.height() + 2;
		painter->drawText(x, y, mSubHeading2);
	}
	else if (!mSubHeading1.isEmpty())
	{
		int x = -title.width() / 2;
		int y = cy - 5;
		painter->drawText(x, y, mTitle);

		x = -heading1.width() / 2;
		y = cy + heading1.height();
		painter->drawText(x, y, mSubHeading1);
	}
	else
	{
		int x = -title.width() / 2;
		int y = cy + (title.height() / 2) - 2;
		painter->drawText(x, y, mTitle);
	}

	painter->restore();
}

void cIoStep::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto widget = event->widget();
	QMenu contextMenu(widget);

	if (mAllowEdit)
	{
		QAction edit("Edit...");
		connect(&edit, &QAction::triggered, this, &cIoStep::editStep);
		contextMenu.addAction(&edit);
		contextMenu.addSeparator();
	}

	QAction before("Insert Step Before...");
	//	connect(&open, &QAction::triggered, this, &cIoStep::openExperiment);
	contextMenu.addAction(&before);

	QAction after("Insert Step After...");
	//	connect(&open, &QAction::triggered, this, &cIoStep::openExperiment);
	contextMenu.addAction(&after);

	contextMenu.exec(event->screenPos());
}

