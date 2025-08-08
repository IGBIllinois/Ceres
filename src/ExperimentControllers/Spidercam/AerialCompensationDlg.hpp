
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

	int32_t getHeight_mm() const;
	int32_t getSensorOffset_mm() const;
	int32_t getDesiredHeight_mm() const;
	int32_t getReferenceHeight_mm() const;

	void setSensorOffset_mm(int32_t offset_mm);
	void setDesiredHeight_mm(int32_t height_mm);
	void setReferenceHeight_mm(int32_t height_mm);

protected:
	void accept() override;
	void reject() override;

private:
	void createControls();
	void createLayout();
	bool compute();

private:
	QLineEdit* mpSensorOffset_mm = nullptr;
	QLineEdit* mpDesiredHeight_mm = nullptr;
	QLineEdit* mpReferenceHeight_mm = nullptr;

	int32_t mHeight_mm = -1; // rfm::INVALID_HEIGHT;

	QPushButton* mpContinue = nullptr;
	QPushButton* mpAbort = nullptr;
};



class cAboveCanopyHeightDlg : public QDialog
{
public:

	enum class eRESULT { NONE, CONTINUE, ABORT };

public:
	cAboveCanopyHeightDlg();
	virtual ~cAboveCanopyHeightDlg();

	int32_t getHeight_mm() const;
	int32_t getReferenceHeight_mm() const;

	void setSensorOffset_mm(int32_t offset_mm);
	void setCanopyHeight_agl(int32_t height_mm);
	void setDesiredHeight_mm(int32_t height_mm);
	void setReferenceHeight_mm(int32_t height_mm);

	eRESULT result();

public slots:
	void showDlg();

protected:
	void accept() override;
	void reject() override;

private:
	void createControls();
	void createLayout();
	bool compute();

private:
	QLineEdit* mpSensorOffset_mm = nullptr;
	QLineEdit* mpCanopyHeight_mm = nullptr;
	QLineEdit* mpDesiredHeight_mm = nullptr;
	QLineEdit* mpReferenceHeight_mm = nullptr;

	int32_t mHeight = -1; // rfm::INVALID_HEIGHT;

	QPushButton* mpContinue = nullptr;
	QPushButton* mpAbort = nullptr;

	eRESULT mResult;
};

