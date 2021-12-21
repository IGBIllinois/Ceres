
#pragma once

#include "../Utilities/Utilities.hpp"

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <Qpicture>
#include <vector>

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

public slots:

    //    void setBorderPenWidth(int width);
//    void setBorderPenColor(const QColor& color);

 //   void setMeasurementPenWidth(int width);
 //   void setMeasurementPenColor(const QColor& color);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
    void drawDollyMarker(QPainter& painter, double height);
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
    QPen   mMeasurementPen;
    QColor mMeasurementColor;

    QPainter mBorder;
    QPen   mBorderPen;
    QColor mBorderColor;

    QPoint mDollyPosition;
    QPen   mDollyPen;
    QBrush mDollyBrush;
    QColor mDollyColor;
    int    mDollyMarkerRadius;
};

