
#pragma once

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
QT_END_NAMESPACE


class cCeresOptionsDlg : public QDialog
{
	Q_OBJECT

public:
	cCeresOptionsDlg(QWidget* parent = nullptr);
	virtual ~cCeresOptionsDlg();

	QString wavFilename() const;
	void setWavFilename(QString filename);

	QString fieldLayoutFilename() const;
	void setFieldLayoutFilename(QString filename);

	QString experimentPath() const;
	void setExperimentPath(QString path);

private slots:
	void browseWavFiles();
	void browseFieldLayout();
	void browseExperimentPath();

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpWavFilename = nullptr;
	QPushButton* mpBrowseWavFile = nullptr;

	QLineEdit* mpFieldLayoutFilename = nullptr;
	QPushButton* mpBrowseFieldLayout = nullptr;

	QLineEdit* mpDefaultExperimentPath = nullptr;
	QPushButton* mpBrowseExperimentPath = nullptr;
};