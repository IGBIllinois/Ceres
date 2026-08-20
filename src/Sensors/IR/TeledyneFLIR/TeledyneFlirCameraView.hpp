
#pragma once

#include "../IrCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QComboBox;
class QGroupBox;
class QPushButton;
class QPaintEvent;
QT_END_NAMESPACE

// Forward Declaration
class cTeledyneFlirCameraModel;

class cTeledyneFlirCameraView : public cIrCameraView
{
	Q_OBJECT

public:
	cTeledyneFlirCameraView(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cTeledyneFlirCameraView();

	virtual void initialize();

	void connectToModel() override;

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

    QLabel* mpThermalRangeLabel = nullptr;
    QLineEdit* mpThermalRange = nullptr;

	QPushButton* mpGrabImage = nullptr;
	QPushButton* mpAutoUpdateImages = nullptr;

private:
	const cTeledyneFlirCameraModel* mpModel;
};