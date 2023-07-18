
#pragma once

#include <QWidget>
#include <QStaticText>
#include <QTextOption>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
    class QStyleOptionButton;
QT_END_NAMESPACE


/**
 * An LED based indicator
 * 
 * A two state indicator with text.  The indicator has the following layout:
 * 
 *      o  Text
 * 
 * The circle will show either the off color/pattern or the on color/pattern.
 */
class QLedIndicator : public QWidget
{
    Q_OBJECT

public:
    QLedIndicator(QWidget* parent = nullptr);
    QLedIndicator(const QString& text, QWidget* parent = nullptr);
	virtual ~QLedIndicator();

	void setText(const QString& text);
	QString text() const;

    bool getState() const;
    void setState(bool state);

    void toggle();

    void setOnColor(QColor onColor);
    void setOnPattern(Qt::BrushStyle onPattern);

    void setOffColor(QColor offColor);
    void setOffPattern(Qt::BrushStyle offPattern);

    int  margin() const;
    void setMargin(int margin);

    /*
     * Sets the size of the LED circle.
     */
    void setLedSize(int size);

signals:
    void stateChanged(bool lit);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
	QStaticText mText;

    bool    mLit;

    QColor  mLedOnColor;
    Qt::BrushStyle mLedOnPattern;

    QColor  mLedOffColor;
    Qt::BrushStyle mLedOffPattern;

    int mLedSize;
    int mMargin;
};



/**
 * A button based indicator (looks similar to a push button w/o button action)
 *
 * A multi-state indicator with text.  The indicator has the following layout:
 *
 *      /--------\
 *      |  Text  |
 *      \--------/
 *
 * The default size is the same as a push button.
 * Each state of the indicator can have its own color/pattern of the background.
 * The defaults are:
 *      UNKNOWN   - gray, solid
 *      ERROR     - red, solid
 *      WARNING   - yellow, solid
 *      ALERT     - amber, solid
 *      INFO      - white, solid
 *      OK        - green, solid
 *      ACTIVE    - dark-green, solid
 */
class QButtonIndicator : public QWidget
{
    Q_OBJECT

public:
    enum eState {UNKNOWN, ERROR, WARNING, ALERT, INFO, OK, ACTIVE};

public:
    QButtonIndicator(QWidget* parent = nullptr);
    QButtonIndicator(const QString& text, QWidget* parent = nullptr);
    virtual ~QButtonIndicator();

    void setText(const QString& text);
    QString text() const;

    eState getState() const;
    void setState(eState state);
    void setState(eState state, const QString& text);

    void setStateColor(eState state, QColor color);
    void setStatePattern(eState state, Qt::BrushStyle pattern);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:
    void stateChanged(eState state);

protected:
    void initStyleOption(QStyleOptionButton* option) const;
    void paintEvent(QPaintEvent* event) override;

private:
    eState  mState = eState::UNKNOWN;
    QString mText;

    mutable QSize mSizeHint;

    QBrush  mUnknownBrush;
    QBrush  mErrorBrush;
    QBrush  mWarningBrush;
    QBrush  mAlertBrush;
    QBrush  mInfoBrush;
    QBrush  mOkBrush;
    QBrush  mActiveBrush;

    int mRadius = 5;
};



