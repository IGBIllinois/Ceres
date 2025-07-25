
#include "QIndicator.hpp"

#include <QApplication>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOption>


/*
extern bool qt_is_gui_used;

Q_GUI_EXPORT int qt_defaultDpiX()
{
	if (QCoreApplication::instance()->testAttribute(Qt::AA_Use96Dpi))
		return 96;

	if (!qt_is_gui_used)
		return 75;

	if (const QScreen* screen = QGuiApplication::primaryScreen())
		return qRound(screen->logicalDotsPerInchX());

	//PI has not been initialised, or it is being initialised. Give a default dpi
	return 100;
}
*/

QT_BEGIN_NAMESPACE
	
Q_GUI_EXPORT int qt_defaultDpiX();

QT_END_NAMESPACE

namespace QStyleHelper
{

#ifdef Q_OS_DARWIN
	static const qreal qstyleBaseDpi = 72;
#else
	static const qreal qstyleBaseDpi = 96;
#endif

	qreal dpi(const QStyleOption* option)
	{
#ifndef Q_OS_DARWIN
		// Prioritize the application override, except for on macOS where
		// we have historically not supported the AA_Use96Dpi flag.
		if (QCoreApplication::testAttribute(Qt::AA_Use96Dpi))
			return 96;
#endif

		// Expect that QStyleOption::QFontMetrics::QFont has the correct DPI set
		if (option)
			return option->fontMetrics.fontDpi();

		// Fall back to historical Qt behavior: hardocded 72 DPI on mac,
		// primary screen DPI on other platforms.
#ifdef Q_OS_DARWIN
		return qstyleBaseDpi;
#else
		return qt_defaultDpiX();
#endif
	}

	qreal dpiScaled(qreal value, qreal dpi)
	{
		return value * dpi / qstyleBaseDpi;
	}
}


/////////////////////////////////////////////////////////////////////
// L E D   I N D I C A T O R
/////////////////////////////////////////////////////////////////////
QLedIndicator::QLedIndicator(const QString& text, QWidget* parent)
:
	QLedIndicator(parent)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);
}

QLedIndicator::QLedIndicator(QWidget* parent)
:
	QWidget(parent), mLit(false)
{
	mLedOnColor	  = Qt::green;
	mLedOnPattern = Qt::SolidPattern;

	mLedOffColor   = Qt::red;
	mLedOffPattern = Qt::SolidPattern;

	mMargin = 10;
	mLedSize = 12;
	setFixedSize(mLedSize + mMargin, mLedSize + mMargin);
}

QLedIndicator::~QLedIndicator()
{}

void QLedIndicator::setText(const QString& text)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);

	update();
}

QString QLedIndicator::text() const
{
	return mText.text();
}

void QLedIndicator::toggle()
{
	mLit = !mLit;
	update();
}

void QLedIndicator::setOnColor(QColor onColor)
{
	mLedOnColor = onColor;
	update();
}

void QLedIndicator::setOnPattern(Qt::BrushStyle onPattern)
{
	mLedOnPattern = onPattern;
	update();
}

void QLedIndicator::setOffColor(QColor offColor)
{
	mLedOffColor = offColor;
	update();
}

void QLedIndicator::setOffPattern(Qt::BrushStyle offPattern)
{
	mLedOffPattern = offPattern;
	update();
}

bool QLedIndicator::getState() const
{
	return mLit;
}

void QLedIndicator::setState(bool state)
{
	mLit = state;
	update();
}

int  QLedIndicator::margin() const { return mMargin;  }

void QLedIndicator::setMargin(int margin)
{
	mMargin = margin < 0 ? 0 : margin;
}

void QLedIndicator::setLedSize(int size)
{
	mLedSize = size < 0 ? 0 : size;

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);

	update();
}

void QLedIndicator::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	mLit ? p.setBrush(QBrush(mLedOnColor, mLedOnPattern)) : p.setBrush(QBrush(mLedOffColor, mLedOffPattern));
	p.drawEllipse(0, 0, mLedSize, mLedSize);
	p.drawStaticText(mLedSize + 5, 0, mText);
}


/////////////////////////////////////////////////////////////////////
// M U L T I   S T A T E   L E D   I N D I C A T O R
/////////////////////////////////////////////////////////////////////
QMultiStateLedIndicator::QMultiStateLedIndicator(const QString& text, QWidget* parent)
	:
	QMultiStateLedIndicator(parent)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);
}

QMultiStateLedIndicator::QMultiStateLedIndicator(QWidget* parent)
	:
	QWidget(parent)
{

	for (auto& style : mLedStyle)
	{
		style.ledColor = Qt::transparent;
		style.ledPattern = Qt::NoBrush;
	}

	mMargin = 10;
	mLedSize = 12;
	setFixedSize(mLedSize + mMargin, mLedSize + mMargin);
}

QMultiStateLedIndicator::~QMultiStateLedIndicator()
{
}

void QMultiStateLedIndicator::setText(const QString& text)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);

	update();
}

QString QMultiStateLedIndicator::text() const
{
	return mText.text();
}

void QMultiStateLedIndicator::setStateColor(uint8_t state, QColor onColor)
{
	mLedStyle[state].ledColor = onColor;
	update();
}

void QMultiStateLedIndicator::setStatePattern(uint8_t state, Qt::BrushStyle onPattern)
{
	mLedStyle[state].ledPattern = onPattern;
	update();
}

uint8_t QMultiStateLedIndicator::getState() const
{
	return mState;
}

void QMultiStateLedIndicator::setState(uint8_t state)
{
	mState = state;
	update();

	emit stateChanged(mState);
}

void QMultiStateLedIndicator::changeState(int state)
{
	if ((state < 0) || (state > 255)) return;
	setState(static_cast<uint8_t>(state));
}

int  QMultiStateLedIndicator::margin() const { return mMargin; }

void QMultiStateLedIndicator::setMargin(int margin)
{
	mMargin = margin < 0 ? 0 : margin;
}

void QMultiStateLedIndicator::setLedSize(int size)
{
	mLedSize = size < 0 ? 0 : size;

	auto text_size = mText.size();
	setFixedSize(mLedSize + mMargin + text_size.width(), mLedSize + mMargin);

	update();
}

void QMultiStateLedIndicator::paintEvent(QPaintEvent* event)
{
	QPainter p(this);

	p.setBrush(QBrush(mLedStyle[mState].ledColor, mLedStyle[mState].ledPattern));
	p.drawEllipse(0, 0, mLedSize, mLedSize);
	p.drawStaticText(mLedSize + 5, 0, mText);
}



/////////////////////////////////////////////////////////////////////
// B U T T O N   I N D I C A T O R
/////////////////////////////////////////////////////////////////////
QButtonIndicator::QButtonIndicator(const QString& text, QWidget* parent)
	:
	QButtonIndicator(parent)
{
	mText = text;
}

QButtonIndicator::QButtonIndicator(QWidget* parent)
	:
	QWidget(parent), mUnknownBrush(Qt::gray, Qt::SolidPattern),
	mErrorBrush(Qt::red, Qt::SolidPattern), mWarningBrush(Qt::yellow, Qt::SolidPattern),
	mAlertBrush(QColor(255, 191,0), Qt::SolidPattern), mInfoBrush(Qt::white, Qt::SolidPattern),
	mOkBrush(Qt::green, Qt::SolidPattern), mActiveBrush(QColor(0, 179, 0), Qt::SolidPattern)
{
}

QButtonIndicator::~QButtonIndicator()
{}

void QButtonIndicator::setText(const QString& text)
{
	mText = text;
	repaint();
}

QString QButtonIndicator::text() const
{
	return mText;
}

QButtonIndicator::eState QButtonIndicator::getState() const
{
	return mState;
}

void QButtonIndicator::setState(eState state)
{
	mState = state;
	repaint();
}

void QButtonIndicator::setState(eState state, const QString& text)
{
	mState = state;
	mText = text;
	repaint();
}

void QButtonIndicator::setStateColor(eState state, QColor color)
{
	switch (mState)
	{
	case UNKNOWN:
		mUnknownBrush.setColor(color);
		break;
	case ERROR:
		mErrorBrush.setColor(color);
		break;
	case WARNING:
		mWarningBrush.setColor(color);
		break;
	case ALERT:
		mAlertBrush.setColor(color);
		break;
	case INFO:
		mInfoBrush.setColor(color);
		break;
	case OK:
		mOkBrush.setColor(color);
		break;
	case ACTIVE:
		mActiveBrush.setColor(color);
		break;
	}
}

void QButtonIndicator::setStatePattern(eState state, Qt::BrushStyle pattern)
{
	switch (mState)
	{
	case UNKNOWN:
		mUnknownBrush.setStyle(pattern);
		break;
	case ERROR:
		mErrorBrush.setStyle(pattern);
		break;
	case WARNING:
		mWarningBrush.setStyle(pattern);
		break;
	case ALERT:
		mAlertBrush.setStyle(pattern);
		break;
	case INFO:
		mInfoBrush.setStyle(pattern);
		break;
	case OK:
		mOkBrush.setStyle(pattern);
		break;
	case ACTIVE:
		mActiveBrush.setStyle(pattern);
		break;
	}
}

//QSize QButtonIndicator::minimumSizeHint() const
//{
//	return sizeHint();
//}

QSize QButtonIndicator::sizeHint() const
{
	if (mSizeHint.isValid())
		return mSizeHint;

	ensurePolished();

	int w = 0, h = 0;

	QStyleOptionButton opt;
	initStyleOption(&opt);

	// calculate contents size...
	QString s(mText);
	bool empty = s.isEmpty();
	if (empty)
		s = QStringLiteral("XXXX");
	QFontMetrics fm = fontMetrics();
	QSize sz = fm.size(Qt::TextShowMnemonic, s);
	if (!empty || !w)
		w += sz.width();
	if (!empty || !h)
		h = qMax(h, sz.height());
	opt.rect.setSize(QSize(w, h));

	mSizeHint = (style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this).
		expandedTo(QApplication::globalStrut()));

	return mSizeHint;

}

void QButtonIndicator::initStyleOption(QStyleOptionButton* option) const
{
	if (!option)
		return;

	option->initFrom(this);
	option->features = QStyleOptionButton::Flat;
	option->state = QStyle::State_Enabled | QStyle::State_On;
	option->text = mText;
}

void QButtonIndicator::paintEvent(QPaintEvent* event)
{
	QStyleOptionButton opt;
	initStyleOption(&opt);

	QStylePainter p(this);

	// Save pen/brush
	QPen oldPen = p.pen();
	QBrush oldBrush = p.brush();

	const qreal dpi = QStyleHelper::dpi(&opt);
	const auto topLevelAdjustment = QStyleHelper::dpiScaled(0.5, dpi);
	const auto bottomRightAdjustment = QStyleHelper::dpiScaled(-1, dpi);

	// Set the background fill color based on the state
	switch (mState)
	{
	case UNKNOWN:
		p.setBrush(mUnknownBrush);
		break;
	case ERROR:
		p.setBrush(mErrorBrush);
		break;
	case WARNING:
		p.setBrush(mWarningBrush);
		break;
	case ALERT:
		p.setBrush(mAlertBrush);
		break;
	case INFO:
		p.setBrush(mInfoBrush);
		break;
	case OK:
		p.setBrush(mOkBrush);
		break;
	case ACTIVE:
		p.setBrush(mActiveBrush);
		break;
	}

	p.setPen(QPen(Qt::black, 0));
	p.drawRoundedRect(QRectF(opt.rect).adjusted(0, 0, -topLevelAdjustment, -topLevelAdjustment), mRadius, mRadius);

	// Restore pen/brush
	p.setPen(oldPen);
	p.setBrush(oldBrush);

	// Draw the text...
	QRect textRect = opt.rect;
	uint tf = Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextHideMnemonic;

	style()->drawItemText(&p, textRect, tf, opt.palette, true, mText, QPalette::ButtonText);
}

