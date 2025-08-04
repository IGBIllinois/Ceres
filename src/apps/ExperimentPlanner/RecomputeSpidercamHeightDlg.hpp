
#pragma once

#include "RappGroundModel.hpp"

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QStackedLayout;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
QT_END_NAMESPACE


class cRecomputeSpidercamHeightDlg : public QDialog
{
	Q_OBJECT

public:
	cRecomputeSpidercamHeightDlg(QWidget* parent = nullptr);
	virtual ~cRecomputeSpidercamHeightDlg();

	int32_t getHeight_mm() const;
	int32_t getReferenceHeight_mm() const;

	void setSensorOffset(int32_t offset_mm);
	void setCanopyHeight_agl(int32_t height_mm);
	void setDesiredHeight_agl(int32_t height_mm);
	void setReferenceHeight_mm(int32_t height_mm);

private slots:
	void accept() override;

private:
	void createControls();
	void createLayout();
	bool compute();

private:
	QLineEdit* mpSensorOffset_mm = nullptr;
	QLineEdit* mpCanopyHeight_mm = nullptr;
	QLineEdit* mpDesiredHeight_mm = nullptr;
	QLineEdit* mpReferenceHeight_mm = nullptr;

	int32_t mHeight = rfm::INVALID_HEIGHT;
};