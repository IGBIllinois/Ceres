
#pragma once

#include "LucidVisionLabsRgbView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
class QToolButton;
QT_END_NAMESPACE

class cLucidVisionLabsRgbModel_Triton;

class cLucidVisionLabsRgbView_Triton : public cLucidVisionLabsRgbView
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbView_Triton(cLucidVisionLabsRgbModel_Triton* pModel, QWidget* parent = nullptr);
	virtual ~cLucidVisionLabsRgbView_Triton();

	void initialize();

signals:
	void activateCamera(int id);

public slots:
	void enableCamera(int id);
	void cameraSelected_1();
	void cameraSelected_2();
	void cameraSelected_3();
	void cameraSelected_4();

private:
	QToolButton* mpCamera1;
	QToolButton* mpCamera2;
	QToolButton* mpCamera3;
	QToolButton* mpCamera4;

protected:
	const cLucidVisionLabsRgbModel* mpModel;
};