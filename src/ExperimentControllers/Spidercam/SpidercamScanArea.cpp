
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

	mHasSecondaryPosition = false;
	QColor color(128, 128, 128, 128);
	mSecondaryDollyPen.setColor(color);
	mSecondaryDollyPen.setWidth(1);
	mSecondaryDollyBrush.setColor(color);
	mSecondaryDollyBrush.setStyle(Qt::SolidPattern);

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

void cSpidercamScanArea::updateSecondaryDollyPosition(bool valid, uint32_t x, uint32_t y)
{
	mHasSecondaryPosition = valid;
	mSecondaryDollyPosition.setX(x);
	mSecondaryDollyPosition.setY(y);
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

		mLayouts.clear();

		for (auto layout : layouts)
		{
			experimentLayout_t expLayout;

			// bounding box information
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

			// caption information
			auto caption = layout["caption"];

			std::string label = caption["label"];
			expLayout.caption.label = QString::fromStdString(label);

			if (caption.contains("color"))
			{
				auto color = caption["color"];
				int r = color["red"];
				int g = color["green"];
				int b = color["blue"];
				int a = color["alpha"];
				expLayout.caption.color = QColor(r, g, b, a);
			}
			else
				expLayout.caption.color = expLayout.color;

			if (caption.contains("font size"))
				expLayout.caption.font_size = caption["font size"];
			else
				expLayout.caption.font_size = 0;

			std::string align = caption["horizontal align"];
			if (align == "left")
				expLayout.caption.horizontal_align = eHorizontalAlignment::LEFT;
			else if (align == "center")
				expLayout.caption.horizontal_align = eHorizontalAlignment::CENTER;
			else if (align == "right")
				expLayout.caption.horizontal_align = eHorizontalAlignment::RIGHT;
			else
			{
			}

			align = caption["vertical align"];
			if (align == "top")
				expLayout.caption.vertical_align = eVerticalAlignment::TOP;
			else if (align == "center")
				expLayout.caption.vertical_align = eVerticalAlignment::CENTER;
			else if (align == "bottom")
				expLayout.caption.vertical_align = eVerticalAlignment::BOTTOM;
			else
			{
			}

			if (caption.contains("orientation (deg)"))
			{
				expLayout.caption.orientation_deg = caption["orientation (deg)"];

				while (expLayout.caption.orientation_deg > 360.0f)
				{
					expLayout.caption.orientation_deg -= 360.0f;
				}

				while (expLayout.caption.orientation_deg < 0.0f)
				{
					expLayout.caption.orientation_deg = 360.0f + expLayout.caption.orientation_deg;
				}
			}
			else
				expLayout.caption.orientation_deg = 0.0f;

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
		mX_Scale = window_height / (mMaxX - mMinX);
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

	if (mHasSecondaryPosition)
	{
		drawDollyMarker(painter, window_height,
			mSecondaryDollyPosition, mSecondaryDollyPen, mSecondaryDollyBrush);
	}

	drawDollyMarker(painter, window_height, mDollyPosition, mDollyPen, mDollyBrush);
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
	double aspectRatio = static_cast<double>(w) / static_cast<double>(h);

	y = height - y - h;

	painter.drawRect(x, y, w, h);

	if (layout.caption.color != layout.color)
		painter.setPen(QPen(layout.caption.color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	QFont font = painter.font();

	if (layout.caption.font_size > 0)
	{
		font.setPointSizeF(layout.caption.font_size);
		painter.setFont(font);
	}

	QFontMetrics metrics(font);

	QPoint textPoint = { 0,0 };

	auto bounds = metrics.boundingRect(layout.caption.label);
	auto textBounds = metrics.tightBoundingRect(layout.caption.label);
	int halfWidth = textBounds.width() / 2;
	int halfHeight = textBounds.height() / 2;
	int deltaWidth = bounds.width() - textBounds.width();
	int deltaHeigth = bounds.height() - textBounds.height();

	// Compute the text center point
	QPoint textLeftCenter = { 0,0 };
	QPoint textOffset = { 0,0 };

	int alignFlags = 0;

	double orientation_rad = layout.caption.orientation_deg * nConstants::DEG_TO_RAD;
	double cos_orientation = cos(orientation_rad);
	double sin_orientation = sin(orientation_rad);

	switch (layout.caption.horizontal_align)
	{
		case eHorizontalAlignment::LEFT:
		{
			int lp = textBounds.width() * abs(cos_orientation);
			int x1 = x - lp;
			textLeftCenter.setX(x1);
			int xh = halfHeight * abs(sin_orientation);
			if (sin_orientation > 0.0)
			{
				xh += halfHeight * abs(sin_orientation);
			}

			textOffset.setX(- xh - 3 * abs(cos_orientation));
			break;
		}
		case eHorizontalAlignment::CENTER:
		{
			int cp = (w / 2);
			int lp = halfWidth * cos_orientation;
			lp += halfHeight * sin_orientation;
			int x1 = x + (cp - lp);
			textLeftCenter.setX(x1);
			textOffset.setX(deltaHeigth * sin_orientation);
			break;
		}
		case eHorizontalAlignment::RIGHT:
		{
			int x1 = x + w;
			textLeftCenter.setX(x1);
			int xh = bounds.height() * abs(sin_orientation);
			textOffset.setX(xh + 3 * abs(cos_orientation));
			break;
		}
	}

	switch (layout.caption.vertical_align)
	{
		case eVerticalAlignment::TOP:
		{
			int y1 = y - textBounds.width() * sin_orientation;
			textLeftCenter.setY(y1);
			textOffset.setY(-3);
			break;
		}
		case eVerticalAlignment::CENTER:
		{
			int cp = (h / 2);
			int lp = halfWidth * sin_orientation;
			int y1 = y + (cp - lp);
			textLeftCenter.setY(y1);
			textOffset.setY(deltaHeigth * sin_orientation);
			break;
		}
		case eVerticalAlignment::BOTTOM:
		{
			int lp = h + halfHeight * abs(sin_orientation);
			int y1 = y + lp;
			textLeftCenter.setY(y1);
			textOffset.setY(halfHeight + deltaHeigth + 3);
			break;
		}
	}

//	textLowerLeftCorner.setX(textCenter.x() - (textBounds.width() / 2) * abs(sin_orientation));
//	textLowerLeftCorner.setY(textCenter.y() - (textBounds.height() / 2) * abs(cos_orientation));

	textPoint = textLeftCenter + textOffset;

	if (layout.caption.orientation_deg != 0.0f)
	{
		painter.translate(textPoint);
		painter.rotate(layout.caption.orientation_deg);
		painter.drawText(QPoint(0,0), layout.caption.label);
	}
	else
	{
		painter.drawText(textPoint, layout.caption.label);
	}

	painter.restore();
}

void cSpidercamScanArea::drawDollyMarker(QPainter& painter, double height,
	const QPoint& pos, const QPen& pen, const QBrush& brush)
{
	int x = mX_Scale * (pos.x() - mMinX) + mX_Offset;
	int y = mY_Scale * (pos.y() - mMinY) + mY_Offset;

	y = height - y;

	QPoint center(x, y);

	painter.setPen(pen);
	painter.drawEllipse(center, 2*mDollyMarkerRadius, 2*mDollyMarkerRadius);

	painter.setBrush(brush);
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
