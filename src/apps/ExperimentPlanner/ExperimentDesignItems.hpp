
#pragma once

#include <QObject>
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


class cConnectedItem : public QGraphicsItem
{
public:
	explicit cConnectedItem(QGraphicsItem* parent = nullptr);

	void setTopPoint(int x, int y);
	virtual void setTopPoint(QPoint p) = 0;

	virtual QPoint getBottomPoint() const = 0;
};


class cFlowArrow : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
	Q_OBJECT

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

class cTerminal : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
	Q_OBJECT

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


/********************************************************************
 *
 * Flow Chart Step: Process Step
 *
 ********************************************************************/

class cProcessStep : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
	Q_OBJECT

public:
	explicit cProcessStep(QGraphicsItem* parent = nullptr);
	explicit cProcessStep(const QString& text, QGraphicsItem* parent = nullptr);

	const QString& title() const;

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

signals:
	void editStep();

public slots:
	void setTitle(const QString& title);
	void setSubHeading1(const QString& heading);
	void setSubHeading2(const QString& heading);
	void setSubHeading3(const QString& heading);
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
	void recomputeBoxSize();

private:
	QPoint mTop;
	QPoint mBottom;

	QFont mFont;

	QString mTitle;
	QString mSubHeading1;
	QString mSubHeading2;
	QString mSubHeading3;

	float mScale = 1.0;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
};


/********************************************************************
 *
 * Flow Chart Step: Input/Output Step
 *
 ********************************************************************/

class cIoStep : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
		Q_OBJECT

public:
	explicit cIoStep(QGraphicsItem* parent = nullptr);
	explicit cIoStep(const QString& text, QGraphicsItem* parent = nullptr);

	const QString& title() const;

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

	bool readOnly() const;
	void setReadOnly(bool read_only);

signals:
	void editStep();

public slots:
	void setTitle(const QString& title);
	void setSubHeading1(const QString& heading);
	void setSubHeading2(const QString& heading);
	void setSubHeading3(const QString& heading);
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
	void recomputeBoxSize();

private:
	QPoint mTop;
	QPoint mBottom;

	QFont mFont;

	QString mTitle;
	QString mSubHeading1;
	QString mSubHeading2;
	QString mSubHeading3;

	float mScale = 1.0;
	int mShift = 5;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
	bool	mAllowEdit = true;
};


