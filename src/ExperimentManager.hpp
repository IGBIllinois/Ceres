
#pragma once

#include <QtWidgets>
#include <filesystem>

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