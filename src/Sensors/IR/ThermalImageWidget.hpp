
#pragma once

#include <QWidget>
#include <QImage>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class cThermalImageWidget : public QWidget
{
	Q_OBJECT

public:
	cThermalImageWidget(QWidget* parent = nullptr);
	virtual ~cThermalImageWidget();

	bool maintainingAspectRatio() const;
	void maintainAspectRatio(bool enable);

	const QImage& getImage() const;
	void setImage(const QImage& image);

	void resizeImage(int width, int height);

	void resizeEvent(QResizeEvent*) override;
	void paintEvent(QPaintEvent* event) override;

private:
	QImage mCurrentImage;
	int mWindowHeight = -1;
	int mWindowWidth = -1;
	double mAspectRatio = 1.0;
	bool mMaintainAspectRatio = false;
};