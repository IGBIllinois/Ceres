
#pragma once


#include <cbdf/AxisCommunicationsParser.hpp>

#include <QOBject>
#include <QBitmap>


class cAxisCommunicationsModel_file : public QObject, public cAxisCommunicationsParser
{
    Q_OBJECT

public:
    cAxisCommunicationsModel_file(QObject* parent = nullptr);
    virtual ~cAxisCommunicationsModel_file();

signals:
	void updateActiveCameraId(int id);
	void updateFramesRate(int frames_per_sec);
	void updateImageSize(int width, int height);
	void onNewImage(const QImage& image);

protected:
	void onActiveCameraId(int id) override;
	void onFramesPerSecond(int frames_per_sec) override;
	void onBitmap(const cBitmapBuffer& buffer) override;
	void onJPEG(const cJpegBuffer& buffer) override;
	void onMpegFrame(const cMpegFrameBuffer& buffer) override;
//	void onBitmap(const QBitmap& in) override;
//	void onJPEG(const QImage& in) override;
//	void onMpegFrame(const QImage& image) override;
	void onImageSize(int width, int height) override;

private:
	QImage mImage;
	int mCameraId = 0;
	int mFramesPerSec = 0;
	int mImageWidth = 0;
	int mImageHeight = 0;

	int numImages = 0;
};

