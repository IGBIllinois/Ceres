
#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <Qt>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE

// Forward Declaration
enum class eExperimentType;

class cConnectedItem : public QGraphicsItem
{
public:
	explicit cConnectedItem(const int id, QGraphicsItem* parent = nullptr);

	const int getID() const;

	void setTopPoint(int x, int y);
	virtual void setTopPoint(QPoint p) = 0;

	virtual QPoint getBottomPoint() const = 0;

	bool isHighlighted() const;
	virtual void setHighlighted(bool highlight);

protected:
	QPoint mTop;
	QPoint mBottom;

	bool mHighlighted = false;

private:
	const int mID;
};


class cFlowArrow : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
	Q_OBJECT

public:
	explicit cFlowArrow(const int id, eExperimentType exp_type, QGraphicsItem* parent = nullptr);

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

signals:
	void insertBefore(int id, int type);

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

private slots:
	void onInsertDelay();
	void onInsertPause();
	void onInsertMovement();
	void onInsertMarker();
	void onInsertHySpexCommand();
	void onInsertReferencePoint();

private:

	eExperimentType mExperimentType;

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
	explicit cTerminal(const int id, QGraphicsItem* parent = nullptr);
	explicit cTerminal(const int id, const QString& text, QGraphicsItem* parent = nullptr);

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
	void setHighlighted(bool highlight) override;


public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
	QFont mFont;

	QString mText;

	int mMinTextWidth = 0;
	int mMinTextHeight = 0;

	float mScale = 1.0;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QBrush  mBackgroundBrush;
	QColor  mBackgroundColor;
	Qt::BrushStyle mBackgroundStyle = Qt::SolidPattern;

	QBrush  mHighlightBrush;
	QColor  mHighlightColor = Qt::yellow;
	Qt::BrushStyle mHightlightStyle = Qt::Dense2Pattern;

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

class cBaseStep : public QObject, public cConnectedItem
{
	Q_INTERFACES(QGraphicsItem)
	Q_OBJECT

public:
	explicit cBaseStep(const int id, eExperimentType exp_type, QGraphicsItem* parent = nullptr);

signals:
	void insertBefore(int id, int type);
	void insertAfter(int id, int type);
	void deleteStep(int id);

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
	void setHighlighted(bool highlight) override;

protected:
	virtual void recomputeBoxSize() = 0;

protected:
	QFont mFont;

	eExperimentType mExperimentType;

	QString mTitle;
	QString mSubHeading1;
	QString mSubHeading2;
	QString mSubHeading3;

	float mScale = 1.0;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QBrush  mBackgroundBrush;
	QColor  mBackgroundColor;
	Qt::BrushStyle mBackgroundStyle = Qt::SolidPattern;

	QBrush  mHighlightBrush;
	QColor  mHighlightColor;
	Qt::BrushStyle mHightlightStyle = Qt::Dense2Pattern;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
};

class cProcessStep : public cBaseStep
{
	Q_OBJECT

public:
	explicit cProcessStep(const int id, eExperimentType exp_type, QGraphicsItem* parent = nullptr);
	explicit cProcessStep(const int id, eExperimentType exp_type, const QString& text, QGraphicsItem* parent = nullptr);

	const QString& title() const;

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

signals:
	void editStep();

public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private slots:
	void onInsertBefore_Delay();
	void onInsertBefore_Pause();
	void onInsertBefore_Movement();
	void onInsertBefore_Marker();
	void onInsertBefore_HySpexCommand();
	void onInsertBefore_ReferencePoint();

	void onInsertAfter_Delay();
	void onInsertAfter_Pause();
	void onInsertAfter_Movement();
	void onInsertAfter_Marker();
	void onInsertAfter_HySpexCommand();
	void onInsertAfter_ReferencePoint();

	void onDeleteStep();

private:
	void recomputeBoxSize() override;
};


/********************************************************************
 *
 * Flow Chart Step: Input/Output Step
 *
 ********************************************************************/

class cIoStep : public cBaseStep
{
	Q_OBJECT

public:
	explicit cIoStep(const int id, eExperimentType exp_type, QGraphicsItem* parent = nullptr);
	explicit cIoStep(const int id, eExperimentType exp_type, const QString& text, QGraphicsItem* parent = nullptr);

	const QString& title() const;

	void setTopPoint(int x, int y);
	void setTopPoint(QPoint p);

	QPoint getBottomPoint() const;

	bool readOnly() const;
	void setReadOnly(bool read_only);

signals:
	void editStep();

/*
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
*/

public:
	QRectF	boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private slots:
	void onInsertBefore_Delay();
	void onInsertBefore_Pause();
	void onInsertBefore_Movement();
	void onInsertBefore_Marker();
	void onInsertBefore_HySpexCommand();
	void onInsertBefore_ReferencePoint();

	void onInsertAfter_Delay();
	void onInsertAfter_Pause();
	void onInsertAfter_Movement();
	void onInsertAfter_Marker();
	void onInsertAfter_HySpexCommand();
	void onInsertAfter_ReferencePoint();

	void onDeleteStep();

private:
	void recomputeBoxSize() override;

private:
/*
	QFont mFont;

	QString mTitle;
	QString mSubHeading1;
	QString mSubHeading2;
	QString mSubHeading3;

	float mScale = 1.0;
	int mShift = 5;
	int mBoxWidth = 50;
	int mBoxHeight = 20;

	QBrush  mBackgroundBrush;
	QColor  mBackgroundColor;
	Qt::BrushStyle mBackgroundStyle;

	QBrush  mHighlightBrush;
	QColor  mHighlightColor;
	Qt::BrushStyle mHightlightStyle;

	QPen    mPen;
	QBrush  mBrush;
	bool    mAntialiased = false;
*/
	int  mShift = 5;

	bool mAllowEdit = true;
};


