
#pragma once

#include "../RgbCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QPaintEvent;
QT_END_NAMESPACE

class cLucidVisionLabsRgbModel;

class cLucidVisionLabsRgbView : public cRgbCameraView
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbView(cLucidVisionLabsRgbModel* pModel, QWidget* parent = nullptr);
	virtual ~cLucidVisionLabsRgbView();

	virtual void initialize();

signals:
	void requestImage();
	void requestImages(bool update_view);

public slots:
	void onSensorNameChanging(QString old_name, QString new_name, QString instance);
	void onModeChange(int mode);
	void onImageSizeChange(int width, int height);
	void onGrabImage();
	void onGrabImages(bool state);
	virtual void imageUpdated(const QImage& image);

protected:
	QLabel* mpModeLabel = nullptr;
	QComboBox* mpMode = nullptr;

	QLabel* mpImageSizeLabel = nullptr;
	QLineEdit* mpImageSize = nullptr;

	QPushButton* mpGrabImage = nullptr;
	QPushButton* mpAutoUpdateImages = nullptr;

protected:
	const cLucidVisionLabsRgbModel* mpModel;
};