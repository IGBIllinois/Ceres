
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class cAboveGroundHeightDlg : public QDialog
{
public:
	cAboveGroundHeightDlg();
	virtual ~cAboveGroundHeightDlg();

	int32_t getSensorOffset_mm() const;
	int32_t getReferenceHeight_mm() const;

	void setSensorOffset_mm(int32_t offset_mm);
	void setReferenceHeight_mm(int32_t height_mm);

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpSensorOffset_mm = nullptr;
	QLineEdit* mpReferenceHeight_mm = nullptr;
};



class cAboveCanopyHeightDlg : public QDialog
{
public:
	cAboveCanopyHeightDlg();
	virtual ~cAboveCanopyHeightDlg();

	int32_t getCanopyHeight_mm() const;
	int32_t getSensorOffset_mm() const;
	int32_t getReferenceHeight_mm() const;

	void setSensorOffset_mm(int32_t offset_mm);
	void setCanopyHeight_mm(int32_t height_mm);
	void setReferenceHeight_mm(int32_t height_mm);

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpSensorOffset_mm = nullptr;
	QLineEdit* mpCanopyHeight_mm = nullptr;
	QLineEdit* mpReferenceHeight_mm = nullptr;
};

