
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

	void connectToModel() override;

signals:
	void saveImage();

signals:
	void requestCameraID(int id);

public slots:
	void onCameraIdChange(int id);
	void onCameraRangeChange(int min_id, int max_id);

protected slots:
	void cameraIdIndexChanged(int id);

protected:
	void createWidgets() override;
	void doControlLayout(QHBoxLayout* pControlLayout) override;

private:
	QLabel* mpCameraIdLabel = nullptr;
	QComboBox* mpCameraId = nullptr;

	int mMinCameraId = 0;
	int mMaxCameraId = 0;

	QPushButton* mpShowCrossHairs = nullptr;

//	QPushButton* mpSaveImage = nullptr;

protected:
	const cAxisCommunicationsModel_F44* mpModel;
};