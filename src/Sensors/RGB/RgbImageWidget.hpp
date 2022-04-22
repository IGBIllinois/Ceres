
#pragma once

#include <QWidget>
#include <QImage>

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

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QImage mCurrentImage;
};