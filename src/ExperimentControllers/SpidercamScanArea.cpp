
#include "SpidercamScanArea.hpp"

#include <QPaintEvent>

cSpidercamScanArea::cSpidercamScanArea(QWidget* parent)
:
	QWidget(parent)
{
	setBackgroundRole(QPalette::Base);

	mMeasurementColor.setRgb(255, 0, 255);
	mMeasurementPen.setColor(mMeasurementColor);
	mMeasurementPen.setWidth(2);

	mBorderColor.setRgb(0, 0, 255);
	mBorderPen.setColor(mBorderColor);
	mBorderPen.setStyle(Qt::SolidLine);
	mBorderPen.setWidth(5);
	mBorder.setPen(mBorderPen);

	mDollyMarkerRadius = 3;
	mDollyColor.setRgb(0,0,255);
	mDollyPen.setColor(mDollyColor);
	mDollyPen.setWidth(1);
	mDollyBrush.setColor(mDollyColor);
	mDollyBrush.setStyle(Qt::SolidPattern);

	mIsRecording = false;
	mpActivePath = nullptr;
}

cSpidercamScanArea::~cSpidercamScanArea()
{
}

QSize cSpidercamScanArea::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize cSpidercamScanArea::sizeHint() const
{
	return QSize(100, 100);
}

bool cSpidercamScanArea::isRecording() const
{
	return mIsRecording;
}

void cSpidercamScanArea::setRecording(bool recording)
{
	mIsRecording = recording;
}

void cSpidercamScanArea::clearRecordedPath()
{
	mMeasurementPaths.clear();
	mpActivePath = nullptr;
}

void cSpidercamScanArea::updateDollyPosition(uint32_t x, uint32_t y)
{
	mDollyPosition.setX(x);
	mDollyPosition.setY(y);

	if (mIsRecording)
	{
		if (mIsRecording.IsRising())
		{
			mMeasurementPaths.push_back(path_t());
			mpActivePath = &(mMeasurementPaths.back());
			mIsRecording.reset();
		}

		mpActivePath->push_back({x,y});
	}
}

void cSpidercamScanArea::updateBounds(double minX, double maxX, double minY, double maxY)
{
	mMinX = minX;
	mMaxX = maxX;
	mMinY = minY;
	mMaxY = maxY;

	mAspectRatio = (mMaxX - mMinX) / (mMaxY - mMinY);
}

void cSpidercamScanArea::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QPen(mBorderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	double w = width();
	double h = height();

	double ideal_height = w / mAspectRatio;
	if (h < ideal_height)
	{
		double ideal_width = mAspectRatio * h;
		mX_Offset = (0.5 * w) - (0.5 * ideal_width);
		mY_Offset = 0.0;
		mX_Scale = ideal_width / (mMaxX - mMinX);
		mY_Scale = h / (mMaxY - mMinY);

		painter.drawRect(mX_Offset, 0, h, h);

	}
	else
	{
		mX_Offset = 0.0;
		mY_Offset = 0.0;
		mX_Scale = w / (mMaxX - mMinX);
		mY_Scale = ideal_height / (mMaxY - mMinY);

		painter.drawRect(0, 0, w, w);
	}

	drawDollyMarker(painter);
	drawPath(painter);
}

void cSpidercamScanArea::drawDollyMarker(QPainter& painter)
{
	int x = mX_Scale * (mDollyPosition.x() - mMinX) + mX_Offset;
	int y = mY_Scale * (mDollyPosition.y() - mMinY) + mY_Offset;
	QPoint center(x, y);

	if (mIsRecording)
	{
		mDollyPen.setColor(mMeasurementColor);
		mDollyBrush.setColor(mMeasurementColor);
	}
	else
	{
		mDollyPen.setColor(mDollyColor);
		mDollyBrush.setColor(mDollyColor);
	}

	painter.setPen(mDollyPen);
	painter.drawEllipse(center, 2*mDollyMarkerRadius, 2*mDollyMarkerRadius);

	painter.setBrush(mDollyBrush);
	painter.drawEllipse(center, mDollyMarkerRadius, mDollyMarkerRadius);

}

void cSpidercamScanArea::drawPath(QPainter& painter)
{
	if (mMeasurementPaths.empty()) return;

	painter.setPen(mMeasurementPen);

	QPainterPath path;

	for (auto& measurementPath : mMeasurementPaths)
	{
		if (measurementPath.empty()) continue;

		path.clear();

		auto point = measurementPath[0];
		int x = mX_Scale * (point.x_mm - mMinX) + mX_Offset;
		int y = mY_Scale * (point.y_mm - mMinY) + mY_Offset;
		path.moveTo(x, y);

		for (int i = 0; i < measurementPath.size(); ++i)
		{
			auto point = measurementPath[i];
			int x = mX_Scale * (point.x_mm - mMinX) + mX_Offset;
			int y = mY_Scale * (point.y_mm - mMinY) + mY_Offset;
			path.lineTo(x, y);
		}

		painter.drawPath(path);
	}

}
