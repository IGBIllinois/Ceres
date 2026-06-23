
#pragma once

#include "AxisCommunicationsView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
class QToolButton;
class QPushButton;
QT_END_NAMESPACE

class cAxisCommunicationsModel_F44;

class cAxisCommunicationsView_F44 : public cAxisCommunicationsView
{
	Q_OBJECT

public:
	cAxisCommunicationsView_F44(cAxisCommunicationsModel_F44* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView_F44();

	void initialize();

signals:
	void activateCamera(int id);

public slots:
	void enableCamera(int id);
	void cameraSelected_1();
	void cameraSelected_2();
	void cameraSelected_3();
	void cameraSelected_4();
	void saveImage();

protected:
	void imageUpdated(const QImage& image) override;

private:
	QToolButton* mpCamera1;
	QToolButton* mpCamera2;
	QToolButton* mpCamera3;
	QToolButton* mpCamera4;

	QPushButton* mpSaveImage = nullptr;

	bool mSaveImage = false;
	int mImageNum = 1;

protected:
	const cAxisCommunicationsModel* mpModel;
};