
#pragma once

#include <QtWidgets>
#include <QDialog>
#include <filesystem>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

class cExperimentManager : public QTreeWidget
{
public:
	cExperimentManager(QWidget* parent = nullptr);

private:
	void loadActiveExperiments();
	void loadArchivedExperiments();

private:
	std::filesystem::path	mActivePath;
	std::filesystem::path	mArchivePath;
};

class cExperimentSelectDlg : public QDialog
{
public:
	cExperimentSelectDlg(QWidget* parent = nullptr);
};
