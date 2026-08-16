
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
	void onTimestamp(uint8_t device_id, uint64_t timestamp_ns) override;
	void onMode(uint8_t device_id, int mode) override;
	void onActiveCameraId(uint8_t device_id, int id) override;
	void onFramesPerSecond(uint8_t device_id, int frames_per_sec) override;
	void onImageSize(uint8_t device_id, int width, int height) override;
	void onLapseTime(uint8_t device_id, int lapse_time_ms) override;

	void onBitmap(uint8_t device_id, const cBitmapBuffer& buffer) override;
	void onJPEG(uint8_t device_id, const cJpegBuffer& buffer) override;
	void onMpegFrame(uint8_t device_id, const cMpegFrameBuffer& buffer) override;

	void onBitmap(uint8_t device_id, uint64_t timestamp_ns, const cBitmapBuffer& buffer) override;
	void onJPEG(uint8_t device_id, uint64_t timestamp_ns, const cJpegBuffer& buffer) override;
	void onMpegFrame(uint8_t device_id, uint64_t timestamp_ns, const cMpegFrameBuffer& buffer) override;

private:
	QImage mImage;
	int mCameraId = 0;
	int mFramesPerSec = 0;
	int mImageWidth = 0;
	int mImageHeight = 0;

	int numImages = 0;
};

