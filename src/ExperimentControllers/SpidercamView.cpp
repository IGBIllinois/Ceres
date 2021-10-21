
#include "SpidercamView.hpp"


cSpidercamView::cSpidercamView()
	:
	mpDrawingArea(nullptr)
{
	mpDrawingArea = new QPainter(this);
	mpDrawingArea->setRenderHint(QPainter::Antialiasing);
	mpDrawingArea->setBackgroundMode(Qt::OpaqueMode);

	mpBorderPen = new QPen(Qt::SolidLine);
	mpBorderPen->setColor(QColor::fromRgb(0,0,255));
}

cSpidercamView::~cSpidercamView()
{
}

QString cSpidercamView::getViewTitle() const
{
	return "Hello";
}

void cSpidercamView::onUpdatedPosition()
{

}

void cSpidercamView::paintEvent(QPaintEvent*)
{
	mpDrawingArea->setPen(*mpBorderPen);
	mpDrawingArea->setBrush(Qt::NoBrush);
}
