
#pragma once

#include "RGB/AxisCommunications/AxisCommunicationsView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


class cAxisCommunicationsView_file : public cAxisCommunicationsView
{
	Q_OBJECT

public:
	cAxisCommunicationsView_file(QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView_file();

	void initialize();
	void connectToModel();

public slots:
	void activeCameraIdUpdated(int id);
	void framesRateUpdated(int frames_per_sec);
	void imageSizeUpdated(int width, int height);
//	void imageUpdated(const QImage& image);

protected:
	virtual void imageUpdated(const QImage& image);

private:
	QLabel*		mpCameraIdLabel = nullptr;
	QLineEdit*	mpCameraId = nullptr;
	QLabel*		mpFrameRateLabel = nullptr;
	QLineEdit*	mpFrameRate = nullptr;
	QLabel*		mpFrameNumberLabel = nullptr;
	QLineEdit*  mpFrameNumber = nullptr;
	QLabel*		mpImageSizeLabel = nullptr;
	QLineEdit*  mpImageSize = nullptr;

	unsigned int mFrameNumber = 0;
};