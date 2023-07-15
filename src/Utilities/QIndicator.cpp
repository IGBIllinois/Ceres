
#include "QIndicator.hpp"
#include <QPainter>

QIndicator::QIndicator(const QString& text, QWidget* parent)
:
	QIndicator(parent)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + 10 + text_size.width(), mLedSize + 10);
}

QIndicator::QIndicator(QWidget* parent)
:
	QWidget(parent), mLit(false)
{
	mLedOnColor	  = Qt::green;
	mLedOnPattern = Qt::SolidPattern;

	mLedOffColor   = Qt::red;
	mLedOffPattern = Qt::SolidPattern;

	mLedSize = 12;
	setFixedSize(mLedSize + 10, mLedSize + 10);
}

QIndicator::~QIndicator()
{}

void QIndicator::setText(const QString& text)
{
	mText.setText(text);

	auto text_size = mText.size();
	setFixedSize(mLedSize + 10 + text_size.width(), mLedSize + 10);

	update();
}

QString QIndicator::text() const
{
	return mText.text();
}

void QIndicator::toggle()
{
	mLit = !mLit;
	update();
}

void QIndicator::setOnColor(QColor onColor)
{
	mLedOnColor = onColor;
	update();
}

void QIndicator::setOnPattern(Qt::BrushStyle onPattern)
{
	mLedOnPattern = onPattern;
	update();
}

void QIndicator::setOffColor(QColor offColor)
{
	mLedOffColor = offColor;
	update();
}

void QIndicator::setOffPattern(Qt::BrushStyle offPattern)
{
	mLedOffPattern = offPattern;
	update();
}

bool QIndicator::getState() const
{
	return mLit;
}

void QIndicator::setState(bool state)
{
	mLit = state;
	update();
}

void QIndicator::setLedSize(int size)
{
	mLedSize = size;

	auto text_size = mText.size();
	setFixedSize(mLedSize + 10 + text_size.width(), mLedSize + 10);

	update();
}

void QIndicator::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	mLit ? p.setBrush(QBrush(mLedOnColor, mLedOnPattern)) : p.setBrush(QBrush(mLedOffColor, mLedOffPattern));
	p.drawEllipse(0, 0, mLedSize, mLedSize);
	p.drawStaticText(mLedSize + 5, 0, mText);
}

