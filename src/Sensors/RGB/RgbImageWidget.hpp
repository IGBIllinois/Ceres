
#pragma once

#include <QWidget>
#include <QImage>
#include <QPen>
#include <QColor>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cRgbImageWidget : public QWidget
{
	Q_OBJECT

public:
	cRgbImageWidget(QWidget* parent = nullptr);
	virtual ~cRgbImageWidget();

	const QImage& getImage() const;
	void setImage(const QImage& image);

public slots:
	void updateCrossHairsColor(int r, int g, int b);
	void showCrossHairs(bool enable);

public slots:
	void resizeEvent(QResizeEvent*) override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	bool mShowCrossHairs = false;

	int mWindowHeight = -1;
	int mWindowWidth = -1;
	double mAspectRatio = 1.0;

	QPen   mPen;
	QColor mColor;

	QImage mCurrentImage;
	QImage mResizedImage;
};