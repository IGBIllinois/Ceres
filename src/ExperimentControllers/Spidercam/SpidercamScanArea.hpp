
#pragma once

#include "../../Utilities/Utilities.hpp"

#include <QWidget>
#include <QPushButton>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <Qpicture>
#include <vector>
#include <string>
#include <array>
#include <tuple>


class cSpidercamScanArea : public QWidget
{
    Q_OBJECT

public:
    enum class eHorizontalAlignment { LEFT, CENTER, RIGHT };
    enum class eVerticalAlignment { TOP, CENTER, BOTTOM };

    struct captionLayout_t
    {
        QColor  color;
        QString label;
        uint8_t font_size = 12;
        eHorizontalAlignment horizontal_align = eHorizontalAlignment::CENTER;
        eVerticalAlignment   vertical_align = eVerticalAlignment::CENTER;
        float orientation_deg = 0;

        bool operator==(const captionLayout_t& rhs) const;
        bool operator!=(const captionLayout_t& rhs) const;
    };

    struct screen_t
    {
        int min_x = 0;
        int min_y = 0;
        int max_x = 0;
        int max_y = 0;

        bool contains(int x, int y) const;
    };

    struct experimentLayout_t
    {
        captionLayout_t caption;

        QColor  color;

        uint32_t x_mm = 0;
        uint32_t y_mm = 0;
        uint32_t height_mm = 0;
        uint32_t width_mm = 0;

        float east_m = 0;
        float north_m = 0;
        float west_m = 0;
        float south_m = 0;

        screen_t pos;

        void computeBounds();

        bool operator==(const experimentLayout_t& rhs) const;
        bool operator!=(const experimentLayout_t& rhs) const;
        bool operator==(const QString& label) const;

        operator bool() const;
    };

public:
    explicit cSpidercamScanArea(QWidget* parent = nullptr);
	~cSpidercamScanArea();

    bool isDollyPositionVisible() const;

    void hideDollyPosition(bool hide = true);
    void showDollyPosition(bool show = true);
    void setDollyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    bool isRecording() const;
    void setRecording(bool recording);
    void clearRecordedPath();
    void setMeasurementColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    void updateBounds(double minX, double maxX, double minY, double maxY);
    void updateDollyPosition(uint32_t x, uint32_t y);

    struct sPosition_t { int32_t x = 0; int32_t y = 0; };
    sPosition_t getDollyPosition() const;

    void loadLayout(const std::string& layout_filename);
    void saveLayout(const std::string& layout_filename);

    void clearLayouts();
    void addLayout(const experimentLayout_t& layout);
    void replaceLayout(const experimentLayout_t& original_layout, const experimentLayout_t& new_layout);

    const std::vector<experimentLayout_t>& getLayouts() const;

    void addMarker(std::string_view label, int x_mm, int y_mm, int z_mm);
    void removeMarkers();

    std::tuple<int, int> toSpiderCamCoordinates(int window_x, int window_y);

public slots:
    void updateSecondaryDollyPosition(bool valid, uint32_t x, uint32_t y);

private slots:
    void onMarkerInfo();

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
    double mBorderWidth = 0.0;
    double mBorderHeight = 0.0;
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
    struct marker_t
    {
        int id = 0;
        QString  label;
        uint32_t x_mm = 0;
        uint32_t y_mm = 0;
        uint32_t z_mm = 0;
    };

    std::vector<marker_t> mMarkers;

    void drawRefPoint(QPainter& painter, double height, const marker_t& marker);

private:
    std::vector<experimentLayout_t> mLayouts;

    void drawLayout(QPainter& painter, double height, experimentLayout_t& layout);

private:
    QPen   mMeasurementPen;
    QColor mMeasurementColor;

    // For drawing the Spidercam border area
    QPen   mBorderPen;
    QColor mBorderColor;
    QBrush mBorderBrush;

    // For drawing the greenway inside the Spidercam area
    QPen   mGreenwayPen;
    QColor mGreenwayColor;
    QBrush mGreenwayBrush;
    std::array<QPoint, 9> mGreenway;

    // For drawing the reference position
    QPen   mMarkerPen;
    QColor mMarkerColor;
    int    mMarkerRadius;
    QPushButton* mpShowMarkerInfo = nullptr;

    // For drawing the dolly position
    bool   mShowDollyPosition = true;
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

