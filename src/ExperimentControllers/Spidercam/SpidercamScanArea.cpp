
#include "SpidercamScanArea.hpp"
#include "../../Utilities/Constants.hpp"

#include <QPaintEvent>

#include <fstream>

#include <nlohmann/json.hpp>


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

bool cSpidercamScanArea::isRecording() const
{
	return mIsRecording;
}

void cSpidercamScanArea::setRecording(bool recording)
{
	mIsRecording = recording;

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

	repaint();
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

	repaint();
}

void cSpidercamScanArea::updateBounds(double minX, double maxX, double minY, double maxY)
{
	mMinX = minX;
	mMaxX = maxX;
	mMinY = minY;
	mMaxY = maxY;

	mAspectRatio = (mMaxX - mMinX) / (mMaxY - mMinY);
}

void cSpidercamScanArea::loadLayout(const std::string& layout_filename)
{
	if (layout_filename.empty())
		return;

	std::ifstream in;
	in.open(layout_filename);
	if (!in.is_open())
		return;

	nlohmann::json layoutDoc;
	try
	{
		in >> layoutDoc;
	}
	catch (const nlohmann::json::parse_error& e)
	{
		return;
	}
	catch (const std::exception& e)
	{
		return;
	}

	try
	{
		auto layouts = layoutDoc["layout"];

		for (auto layout : layouts)
		{
			experimentLayout_t expLayout;

			std::string label = layout["label"];
			expLayout.label = QString::fromStdString(label);

			auto color = layout["color"];
			int r = color["red"];
			int g = color["green"];
			int b = color["blue"];
			int a = color["alpha"];
			expLayout.color = QColor(r, g, b, a);

			uint32_t east_m = layout["east (m)"];
			uint32_t north_m = layout["north (m)"];
			uint32_t west_m = layout["west (m)"];
			uint32_t south_m = layout["south (m)"];

			expLayout.x_mm = north_m * nConstants::M_TO_MM;
			expLayout.y_mm = west_m * nConstants::M_TO_MM;
			expLayout.height_mm = (east_m - west_m) * nConstants::M_TO_MM;
			expLayout.width_mm = (south_m - north_m) * nConstants::M_TO_MM;

			mLayouts.emplace_back(expLayout);
		}

	}
	catch (const std::exception& e)
	{
		return;
	}
}

void cSpidercamScanArea::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QPen(mBorderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	QFont font = painter.font();
	QFontMetrics metrics(font);
	auto w1Bounds = metrics.tightBoundingRect("W1X");
	auto w2Bounds = metrics.tightBoundingRect("XW2");
	auto w3Bounds = metrics.tightBoundingRect("XW3");
	auto w4Bounds = metrics.tightBoundingRect("W4X");

	double window_width = width();
	double window_height = height();

	double test_height = (window_width - w1Bounds.width() - w2Bounds.width()) / mAspectRatio;
	if (window_height < test_height)
	{
		double ideal_width = mAspectRatio * window_height;
		double l = window_width - w1Bounds.width() - w2Bounds.width();

		mX_Offset = 0.5 * (l - ideal_width) + w1Bounds.width();
		mY_Offset = 0.0;
		mX_Scale = ideal_width / (mMaxX - mMinX);
		mY_Scale = window_height / (mMaxY - mMinY);

		painter.drawRect(mX_Offset, mY_Offset, window_height, window_height);

		auto xBounds = metrics.tightBoundingRect("X");
		painter.drawText(QPoint(mX_Offset - w1Bounds.width(), window_height), QString("W1"));
		painter.drawText(QPoint(mX_Offset + window_height + xBounds.width(), window_height), QString("W2"));
		painter.drawText(QPoint(mX_Offset + window_height + xBounds.width(), xBounds.height()), QString("W3"));
		painter.drawText(QPoint(mX_Offset - w4Bounds.width(), w4Bounds.height()), QString("W4"));
	}
	else
	{
		double ideal_height = (window_width / mAspectRatio) - w1Bounds.height() - w4Bounds.height() - 6;
		double l;

		if (window_width > window_height)
			l = window_height - w1Bounds.height() - w4Bounds.height() - 6;
		else
			l = window_width - w1Bounds.height() - w4Bounds.height() - 6;

		mX_Offset = 0.5 * (window_width - ideal_height);
		mY_Offset = 0.5 * (window_height - l);	//w4Bounds.height() + 3;
		mX_Scale = l / (mMaxX - mMinX);
		mY_Scale = l / (mMaxY - mMinY);

		painter.drawRect(mX_Offset, mY_Offset, l, l);

		double y = mY_Offset + l + w1Bounds.height() + 3;
		painter.drawText(QPoint(mX_Offset, y), QString("W1"));
		painter.drawText(QPoint(mX_Offset + l - w3Bounds.width(), y), QString("W2"));
		painter.drawText(QPoint(mX_Offset + l - w3Bounds.width(), mY_Offset - 3), QString("W3"));
		painter.drawText(QPoint(mX_Offset, mY_Offset - 3), QString("W4"));

		window_height = l;
	}

	for (const auto& layout : mLayouts)
	{
		drawLayout(painter, window_height, layout);
	}

	drawDollyMarker(painter, window_height);
	drawPath(painter, window_height);
}


void cSpidercamScanArea::drawLayout(QPainter& painter, double height, const experimentLayout_t& layout)
{
	painter.save();
	painter.setPen(QPen(layout.color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	int x = mX_Scale * (layout.x_mm - mMinX) + mX_Offset;
	int y = mY_Scale * (layout.y_mm - mMinY) + mY_Offset;
	int w = mX_Scale * layout.width_mm;
	int h = mY_Scale * layout.height_mm;

	y = height - y - h;

	painter.drawRect(x, y, w, h);

	QFont font = painter.font();
	QFontMetrics metrics(font);

	painter.drawText(QPoint(x + 0.5 * w, y), layout.label);

	painter.restore();
}

void cSpidercamScanArea::drawDollyMarker(QPainter& painter, double height)
{
	int x = mX_Scale * (mDollyPosition.x() - mMinX) + mX_Offset;
	int y = mY_Scale * (mDollyPosition.y() - mMinY) + mY_Offset;

	y = height - y;

	QPoint center(x, y);

	painter.setPen(mDollyPen);
	painter.drawEllipse(center, 2*mDollyMarkerRadius, 2*mDollyMarkerRadius);

	painter.setBrush(mDollyBrush);
	painter.drawEllipse(center, mDollyMarkerRadius, mDollyMarkerRadius);

}

void cSpidercamScanArea::drawPath(QPainter& painter, double height)
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
		y = height - y;
		path.moveTo(x, y);

		for (int i = 0; i < measurementPath.size(); ++i)
		{
			auto point = measurementPath[i];
			int x = mX_Scale * (point.x_mm - mMinX) + mX_Offset;
			int y = mY_Scale * (point.y_mm - mMinY) + mY_Offset;
			y = height - y;
			path.lineTo(x, y);
		}

		painter.drawPath(path);
	}

}
