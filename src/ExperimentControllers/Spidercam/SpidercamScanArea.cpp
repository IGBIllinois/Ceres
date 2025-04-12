
#include "SpidercamScanArea.hpp"
#include "../../Utilities/Constants.hpp"

#include <nlohmann/json.hpp>

#include <QPaintEvent>

#include <fstream>
#include <algorithm>


bool cSpidercamScanArea::captionLayout_t::operator==(const captionLayout_t& rhs) const
{
	return (color == rhs.color) && (label == rhs.label) && (font_size == rhs.font_size)
		&& (horizontal_align == rhs.horizontal_align) && (vertical_align == rhs.vertical_align)
		&& (orientation_deg == rhs.orientation_deg);
}

bool cSpidercamScanArea::captionLayout_t::operator!=(const captionLayout_t& rhs) const
{
	return !operator==(rhs);
}

bool cSpidercamScanArea::screen_t::contains(int x, int y) const
{
	if ((x < min_x) || (x > max_x)) return false;
	if ((y < min_y) || (y > max_y)) return false;

	return true;
}

void cSpidercamScanArea::experimentLayout_t::computeBounds()
{
	x_mm = north_m * nConstants::M_TO_MM;
	y_mm = west_m * nConstants::M_TO_MM;
	height_mm = (east_m - west_m) * nConstants::M_TO_MM;
	width_mm = (south_m - north_m) * nConstants::M_TO_MM;
}

bool cSpidercamScanArea::experimentLayout_t::operator==(const experimentLayout_t& rhs) const
{
	return (x_mm == rhs.x_mm) && (y_mm == rhs.y_mm) && (height_mm == rhs.height_mm) && (width_mm == rhs.width_mm)
		&& (east_m == rhs.east_m) && (north_m == rhs.north_m) && (west_m == rhs.west_m) && (south_m == rhs.south_m)
		&& (color == rhs.color) && (caption == rhs.caption);
}

bool cSpidercamScanArea::experimentLayout_t::operator!=(const experimentLayout_t& rhs) const
{
	return !operator==(rhs);
}

bool cSpidercamScanArea::experimentLayout_t::operator==(const QString& label) const
{
	return caption.label == label;
}

cSpidercamScanArea::experimentLayout_t::operator bool() const
{
	return (height_mm != 0) && (width_mm != 0);
}

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
	mBorderPen.setCapStyle(Qt::RoundCap);
	mBorderPen.setJoinStyle(Qt::RoundJoin);
	mBorderPen.setWidth(1);
	mBorderBrush.setColor(mBorderColor);
	mBorderBrush.setStyle(Qt::NoBrush);

	mGreenwayColor.setRgb(59, 122, 87);	// Amazon Green
	mGreenwayColor.setRgb(176, 191, 26);	// Acid Green
	mGreenwayPen.setColor(mGreenwayColor);;
	mGreenwayPen.setStyle(Qt::DotLine);

/*
	Different Qt pen style to try
	SolidLine,
	DashLine,
	DotLine,
	DashDotLine,
	DashDotDotLine,
*/

	mGreenwayPen.setCapStyle(Qt::RoundCap);
	mGreenwayPen.setJoinStyle(Qt::RoundJoin);
	mGreenwayPen.setWidth(1);
	mGreenwayBrush.setColor(mGreenwayColor);;
	mGreenwayBrush.setStyle(Qt::Dense7Pattern);

/*
	Different Qt brush style to try:

	SolidPattern,
	Dense1Pattern,
	Dense2Pattern,
	Dense3Pattern,
	Dense4Pattern,
	Dense5Pattern,
	Dense6Pattern,
	Dense7Pattern,
	HorPattern,
	VerPattern,
	CrossPattern,
	BDiagPattern,
	FDiagPattern,
	DiagCrossPattern,
*/

	mGreenway[0] = QPoint(88748, 0);
	mGreenway[1] = QPoint(88748, 94606);
	mGreenway[2] = QPoint(0, 94606);
	mGreenway[3] = QPoint(0, 102642);
	mGreenway[4] = QPoint(200000, 102642);
	mGreenway[5] = QPoint(200000, 94606);
	mGreenway[6] = QPoint(98405, 94606);
	mGreenway[7] = QPoint(98405, 0);
	mGreenway[8] = mGreenway[0];

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

bool cSpidercamScanArea::isDollyPositionVisible() const
{
	return mShowDollyPosition;
}

void cSpidercamScanArea::hideDollyPosition(bool hide)
{
	mShowDollyPosition = !hide;
}

void cSpidercamScanArea::showDollyPosition(bool show)
{
	mShowDollyPosition = show;
}

void cSpidercamScanArea::setDollyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	mDollyColor.setRgb(r, g, b, a);
	mDollyPen.setColor(mDollyColor);
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

void cSpidercamScanArea::setMeasurementColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	mMeasurementColor.setRgb(r, g, b, a);
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

		if (mpActivePath)
			mpActivePath->push_back({x,y});
	}

	repaint();
}

cSpidercamScanArea::sPosition_t cSpidercamScanArea::getDollyPosition() const
{
	return { mDollyPosition.x(), mDollyPosition.y() };
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

	mGreenway[0] = QPoint(88748, mMinY);
	mGreenway[1] = QPoint(88748, 94606);
	mGreenway[2] = QPoint(mMinX, 94606);
	mGreenway[3] = QPoint(mMinX, 102642);
	mGreenway[4] = QPoint(mMaxX, 102642);
	mGreenway[5] = QPoint(mMaxX, 94606);
	mGreenway[6] = QPoint(98405, 94606);
	mGreenway[7] = QPoint(98405, mMinY);
	mGreenway[8] = mGreenway[0];
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

			expLayout.east_m = layout["east (m)"];
			expLayout.north_m = layout["north (m)"];
			expLayout.west_m = layout["west (m)"];
			expLayout.south_m = layout["south (m)"];

			expLayout.computeBounds();

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

void cSpidercamScanArea::saveLayout(const std::string& layout_filename)
{
	if (layout_filename.empty())
		return;

	nlohmann::json layouts;

	for (const auto& expLayout : mLayouts)
	{
		nlohmann::json layout;

		// bounding box information
		nlohmann::json color;

		int r = expLayout.color.red();
		int g = expLayout.color.green();
		int b = expLayout.color.blue();
		int a = expLayout.color.alpha();

		color["red"]   = r;
		color["green"] = g;
		color["blue"]  = b;
		color["alpha"] = a;

		layout["color"] = color;

		layout["east (m)"] = expLayout.east_m;
		layout["north (m)"] = expLayout.north_m;
		layout["west (m)"] = expLayout.west_m;
		layout["south (m)"] = expLayout.south_m;

		// caption information
		nlohmann::json caption;

		caption["label"] = expLayout.caption.label.toStdString();

		int cr = expLayout.caption.color.red();
		int cg = expLayout.caption.color.green();
		int cb = expLayout.caption.color.blue();
		int ca = expLayout.caption.color.alpha();

		if ((r != cr) || (g != cg) || (b != cb) || (a != ca))
		{
			nlohmann::json color;

			int r = expLayout.caption.color.red();
			int g = expLayout.caption.color.green();
			int b = expLayout.caption.color.blue();
			int a = expLayout.caption.color.alpha();

			color["red"] = r;
			color["green"] = g;
			color["blue"] = b;
			color["alpha"] = a;

			caption["color"] = color;
		}

		if (expLayout.caption.font_size != 0)
		{
			caption["font size"] = expLayout.caption.font_size;
		}

		switch (expLayout.caption.horizontal_align)
		{
		case eHorizontalAlignment::LEFT:
			caption["horizontal align"] = "left";
			break;
		case eHorizontalAlignment::CENTER:
			caption["horizontal align"] = "center";
			break;
		case eHorizontalAlignment::RIGHT:
			caption["horizontal align"] = "right";
			break;
		}

		switch (expLayout.caption.vertical_align)
		{
		case eVerticalAlignment::TOP:
			caption["vertical align"] = "top";
			break;
		case eVerticalAlignment::CENTER:
			caption["vertical align"] = "center";
			break;
		case eVerticalAlignment::BOTTOM:
			caption["vertical align"] = "bottom";
			break;
		}

		if (expLayout.caption.orientation_deg != 0.0f)
		{
			caption["orientation (deg)"] = expLayout.caption.orientation_deg;
		}

		layout["caption"] = caption;

		layouts.push_back(layout);
	}

	nlohmann::json layoutDoc;

	layoutDoc["layout"] = layouts;

	std::ofstream out;
	out.open(layout_filename, std::ios::trunc);
	if (!out.is_open())
		return;

	out << std::setw(4) << layoutDoc << std::endl;
}

void cSpidercamScanArea::clearLayouts()
{
	mLayouts.clear();

	repaint();
}

void cSpidercamScanArea::addLayout(const experimentLayout_t& layout)
{
	mLayouts.push_back(layout);

	repaint();
}

void cSpidercamScanArea::replaceLayout(const experimentLayout_t& original_layout, const experimentLayout_t& new_layout)
{
	auto it = std::find(mLayouts.begin(), mLayouts.end(), original_layout);

	if (it == mLayouts.end())
	{
		mLayouts.push_back(new_layout);
	}
	else
		*it = new_layout;

	repaint();
}

const std::vector<cSpidercamScanArea::experimentLayout_t>& cSpidercamScanArea::getLayouts() const
{
	return mLayouts;
}

std::tuple<int, int> cSpidercamScanArea::toSpiderCamCoordinates(int window_x, int window_y)
{
	int x = mMinX + (window_x - mX_Offset) / mX_Scale;
	int y = mMaxY - (window_y - mY_Offset) / mY_Scale;

	return { x, y };
}

void cSpidercamScanArea::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

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

		// Draw the greenway first so the border remains a solid color
		std::array<QPoint, 9> greenway;

		for (int i = 0; i < mGreenway.size(); ++i)
		{
			QPoint& point = mGreenway[i];
			int x = mX_Scale * (point.x() - mMinX) + mX_Offset;
			int y = mY_Scale * (point.y() - mMinY) + mY_Offset;
			greenway[i] = QPoint(x, window_height-y);
		}

		painter.setPen(mGreenwayPen);
		painter.setBrush(mGreenwayBrush);
		painter.drawPolygon(greenway.data(), 8);

		// Draw the spidercam border and tower markings
		mBorderWidth  = window_height;
		mBorderHeight = window_height;
		painter.setPen(mBorderPen);
		painter.setBrush(mBorderBrush);
		painter.drawRect(mX_Offset, mY_Offset, mBorderWidth, mBorderHeight);

		auto xBounds = metrics.tightBoundingRect("X");
		painter.drawText(QPoint(mX_Offset - w1Bounds.width(), mBorderHeight), QString("W1"));
		painter.drawText(QPoint(mX_Offset + mBorderWidth + xBounds.width(), mBorderHeight), QString("W2"));
		painter.drawText(QPoint(mX_Offset + mBorderWidth + xBounds.width(), xBounds.height()), QString("W3"));
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

		// Draw the greenway first so the border remains a solid color
		std::array<QPoint, 9> greenway;

		for (int i = 0; i < mGreenway.size(); ++i)
		{
			QPoint& point = mGreenway[i];
			int x = mX_Scale * (point.x() - mMinX) + mX_Offset;
			int y = mY_Scale * (point.y() - mMinY) + mY_Offset;
			greenway[i] = QPoint(x, window_height - y);
		}

		painter.setPen(mGreenwayPen);
		painter.setBrush(mGreenwayBrush);
		painter.drawPolygon(greenway.data(), 8);

		// Draw the spidercam border and tower markings
		mBorderWidth = l;
		mBorderHeight = l;
		painter.setPen(mBorderPen);
		painter.setBrush(mBorderBrush);
		painter.drawRect(mX_Offset, mY_Offset, mBorderWidth, mBorderHeight);

		double y = mY_Offset + mBorderHeight + w1Bounds.height() + 3;
		painter.drawText(QPoint(mX_Offset, y), QString("W1"));
		painter.drawText(QPoint(mX_Offset + mBorderWidth - w3Bounds.width(), y), QString("W2"));
		painter.drawText(QPoint(mX_Offset + mBorderWidth - w3Bounds.width(), mY_Offset - 3), QString("W3"));
		painter.drawText(QPoint(mX_Offset, mY_Offset - 3), QString("W4"));
	}

	for (auto& layout : mLayouts)
	{
		drawLayout(painter, window_height, layout);
	}

	if (mShowDollyPosition && mHasSecondaryPosition)
	{
		drawDollyMarker(painter, window_height,
			mSecondaryDollyPosition, mSecondaryDollyPen, mSecondaryDollyBrush);
	}

	if (mShowDollyPosition)
		drawDollyMarker(painter, window_height, mDollyPosition, mDollyPen, mDollyBrush);

	drawPath(painter, window_height);
}


void cSpidercamScanArea::drawLayout(QPainter& painter, double height, experimentLayout_t& layout)
{
	painter.save();
	painter.setPen(QPen(layout.color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	int field_offset_x = layout.x_mm - mMinX;
	int field_offset_y = layout.y_mm - mMinY;

	int x = mX_Scale * (layout.x_mm - mMinX) + mX_Offset;
	int y = mY_Scale * (layout.y_mm - mMinY) + mY_Offset;
	int w = mX_Scale * layout.width_mm;
	int h = mY_Scale * layout.height_mm;
	double aspectRatio = static_cast<double>(w) / static_cast<double>(h);

	y = height - y - h;

	if (parent())
	{
		auto p1 = mapToParent(QPoint(x, y));
		auto p2 = mapToParent(QPoint(x + w, y + h));
		layout.pos.min_x = p1.x();
		layout.pos.min_y = p1.y();
		layout.pos.max_x = p2.x();
		layout.pos.max_y = p2.y();
	}
	else
	{
		layout.pos.min_x = x;
		layout.pos.min_y = y;
		layout.pos.max_x = x + w;
		layout.pos.max_y = y + h;
	}

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
