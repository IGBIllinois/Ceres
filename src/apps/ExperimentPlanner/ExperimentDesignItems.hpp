
#pragma once

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QFont>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE


class cFlowArrow : public QGraphicsItem
{
public:
	explicit cFlowArrow(QGraphicsItem* parent = nullptr);

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

public slots:
	void setScale(int scale);
	void setPen(const QPen& pen);
	void setBrush(const QBrush& brush);
	void setAntialiased(bool antialiased);

public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
	QPoint mTop;
	QPoint mBottom;

	float mScale = 1.0;
	int mLineLength = 30;
	int mArrowHeadSize = 10;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
};



/********************************************************************
 *
 * Flow Chart Step: Terminal
 *
 ********************************************************************/

class cTerminal : public QGraphicsItem
{
public:
	explicit cTerminal(QGraphicsItem* parent = nullptr);
	explicit cTerminal(const QString& text, QGraphicsItem* parent = nullptr);

	const QString& text() const;
	void setText(const QString& text);

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

public slots:
	void setScale(int scale);
	void setPen(const QPen& pen);
	void setBrush(const QBrush& brush);
	void setFont(const QFont& font);
	void setAntialiased(bool antialiased);

public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
	QPoint mTop;
	QPoint mBottom;

	QFont mFont;

	QString mText;

	int mMinTextWidth = 0;
	int mMinTextHeight = 0;

	float mScale = 1.0;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
};

class cStartTerminal : public cTerminal
{
public:
	explicit cStartTerminal(QGraphicsItem* parent = nullptr);

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
};

class cEndTerminal : public cTerminal
{
public:
	explicit cEndTerminal(QGraphicsItem* parent = nullptr);

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
};


class cProcessStep : public QGraphicsItem
{
public:
	explicit cProcessStep(QGraphicsItem* parent = nullptr);
	explicit cProcessStep(const QString& text, QGraphicsItem* parent = nullptr);

	const QString& title() const;
	void setTitle(const QString& text);

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

public slots:
	void setScale(int scale);
	void setPen(const QPen& pen);
	void setBrush(const QBrush& brush);
	void setFont(const QFont& font);
	void setAntialiased(bool antialiased);

public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
	QPoint mTop;
	QPoint mBottom;

	QFont mFont;

	int mMinTextWidth = 0;
	int mMinTextHeight = 0;

	QString mTitle;
	QString mSubTitle;

	float mScale = 1.0;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
};

