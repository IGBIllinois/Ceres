
#pragma once

#include "../../Utilities/Utilities.hpp"

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <Qpicture>
#include <vector>
#include <string>

class cSpidercamScanArea : public QWidget
{
    Q_OBJECT

public:
    explicit cSpidercamScanArea(QWidget* parent = nullptr);
	~cSpidercamScanArea();

    bool isRecording() const;
    void setRecording(bool recording);
    void clearRecordedPath();

    void updateBounds(double minX, double maxX, double minY, double maxY);
    void updateDollyPosition(uint32_t x, uint32_t y);

    void loadLayout(const std::string& layout_filename);

public slots:
    void updateSecondaryDollyPosition(bool valid, uint32_t x, uint32_t y);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
    void drawDollyMarker(QPainter& painter, double height, 
        const QPoint& pos, const QPen& pen, const QBrush& brush);

    void drawPath(QPainter& painter, double height);

private:
    double mMinX = 0;
    double mMaxX = 0;
    double mMinY = 0;
    double mMaxY = 0;
    double mAspectRatio = 1.0;
    double mX_Offset = 0.0;
    double mY_Offset = 0.0;
    double mX_Scale = 1.0;
    double mY_Scale = 1.0;
    edge_detect<bool> mIsRecording;

    struct location
    {
        uint32_t x_mm;
        uint32_t y_mm;

        location(uint32_t x, uint32_t y) : x_mm(x), y_mm(y) {}
    };

    typedef std::vector<location> path_t;
    std::vector<path_t> mMeasurementPaths;
    path_t* mpActivePath;

private:
    enum class eHorizontalAlignment { LEFT, CENTER, RIGHT };
    enum class eVerticalAlignment { TOP, CENTER, BOTTOM };

    struct captionLayout_t
    {
        QColor  color;
        QString label;
        uint8_t font_size;
        eHorizontalAlignment horizontal_align;
        eVerticalAlignment   vertical_align;
        float orientation_deg;
    };

    struct experimentLayout_t
    {
        captionLayout_t caption;

        QColor  color;
        uint32_t x_mm;
        uint32_t y_mm;
        uint32_t height_mm;
        uint32_t width_mm;
    };

    std::vector<experimentLayout_t> mLayouts;

    void drawLayout(QPainter& painter, double height, const experimentLayout_t& layout);

private:
    QPen   mMeasurementPen;
    QColor mMeasurementColor;

    QPen   mBorderPen;
    QColor mBorderColor;

    QPoint mDollyPosition;
    QPen   mDollyPen;
    QBrush mDollyBrush;
    QColor mDollyColor;
    int    mDollyMarkerRadius;

    bool   mHasSecondaryPosition;
    QPoint mSecondaryDollyPosition;
    QPen   mSecondaryDollyPen;
    QBrush mSecondaryDollyBrush;
};

