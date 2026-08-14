
#pragma once

#include "AxisCommunicationsView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
class QLabel;
class QLineEdit;
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

	void initialize() override;

	void showCrossHairButton(bool show);

signals:
	void activateCamera(int id);
	void saveImage();

public slots:
	void onCameraIdChange(int id);
	void enableCamera(int id);
	void cameraSelected_1();
	void cameraSelected_2();
	void cameraSelected_3();
	void cameraSelected_4();

protected:
	void imageUpdated(const QImage& image) override;

private:
	QToolButton* mpCamera1 = nullptr;
	QToolButton* mpCamera2 = nullptr;
	QToolButton* mpCamera3 = nullptr;
	QToolButton* mpCamera4 = nullptr;

	QLabel* mpCameraIdLabel = nullptr;
	QLineEdit* mpCameraId = nullptr;

	QPushButton* mpShowCrossHairs = nullptr;

	QPushButton* mpSaveImage = nullptr;

protected:
	const cAxisCommunicationsModel* mpModel;
};