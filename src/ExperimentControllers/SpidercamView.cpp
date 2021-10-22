
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

void cSpidercamView::configure(const nlohmann::json& jsonCfg)
{
	try
	{
		mMinX_mm = jsonCfg["min X position (m)"] * M_TO_MM;
		mMaxX_mm = jsonCfg["max X position (m)"] * M_TO_MM;
		mMinY_mm = jsonCfg["min Y position (m)"] * M_TO_MM;
		mMaxY_mm = jsonCfg["max Y position (m)"] * M_TO_MM;
		mMinHeight_mm = jsonCfg["min height (m)"] * M_TO_MM;
		mMaxHeight_mm = jsonCfg["max height (m)"] * M_TO_MM;
	}
	catch (const std::exception& e)
	{
	}
}

void cSpidercamView::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;
}

void cSpidercamView::updatedPosition(spidercam::sPosition pos)
{
	repaint();
}

void cSpidercamView::paintEvent(QPaintEvent* event)
{
	mpDrawingArea->setPen(*mpBorderPen);
	mpDrawingArea->setBrush(Qt::NoBrush);
	mpDrawingArea->drawRect(0,0,100,100);

	cExperimentControlView::paintEvent(event);
}
