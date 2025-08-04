
#pragma once

#include "Sound.hpp"

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
QT_END_NAMESPACE


class cCeresOptionsDlg : public QDialog
{
	Q_OBJECT

public:
	cCeresOptionsDlg(QWidget* parent = nullptr);
	virtual ~cCeresOptionsDlg();

	QString endOfExperimentWavFilename() const;
	void setEndOfExperimentWavFilename(QString filename);

	QString experimentErrorWavFilename() const;
	void setExperimentErrorWavFilename(QString filename);

	QString fieldLayoutFilename() const;
	void setFieldLayoutFilename(QString filename);

	QString experimentPath() const;
	void setExperimentPath(QString path);

private slots:
	void browseEndOfExperimentWavFile();
	void browseExperimentErrorWavFile();
	void browseFieldLayout();
	void browseExperimentPath();

	void testEndOfExperimentWavFiles();
	void testExperimentErrorWavFiles();

private:
	void createControls();
	void createLayout();

private:
	QLineEdit* mpEndOfExperimentWavFilename = nullptr;
	QPushButton* mpBrowseEndOfExperimentWavFile = nullptr;
	QPushButton* mpTestEndOfExperimentWavFile = nullptr;
	QComboBox* mpEndOfExperimentAudioID = nullptr;

	QLineEdit* mpExperimentErrorWavFilename = nullptr;
	QPushButton* mpBrowseExperimentErrorWavFile = nullptr;
	QPushButton* mpTestExperimentErrorWavFile = nullptr;
	QComboBox* mpExperimentErrorAudioID = nullptr;

	QLineEdit* mpFieldLayoutFilename = nullptr;
	QPushButton* mpBrowseFieldLayout = nullptr;

	QLineEdit* mpDefaultExperimentPath = nullptr;
	QPushButton* mpBrowseExperimentPath = nullptr;

	cSound mTestSound;
};