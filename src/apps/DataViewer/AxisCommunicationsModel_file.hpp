
#pragma once


#include "AxisCommunicationsParser.hpp"

#include <QOBject>
#include <QBitmap>


class cAxisCommunicationsModel_file : public QObject, public cAxisCommunicationsParser
{
    Q_OBJECT

public:
    cAxisCommunicationsModel_file(QObject* parent = nullptr);
    virtual ~cAxisCommunicationsModel_file();

signals:
    void onNewImage(const QImage& image);

protected:
	void onActiveCameraId(int id) override;
	void onFramesPerSecond(int frames_per_sec) override;
	void onBitmap(const QBitmap& in) override;
	void onJPEG(const QImage& in) override;
	void onMpegFrame(const QImage& image) override;
	void onImageSize(const axis::sImageSize_t& in) override;

private:
	QImage mImage;

	int numImages = 0;
};

