
#pragma once

#include <QtWidgets>
#include <QString>

#include <nlohmann/json.hpp>
#include <filesystem>

class cExperimentTreeItem : public QTreeWidgetItem
{
public:
	cExperimentTreeItem(QTreeWidget* parent, const QString& text);
	cExperimentTreeItem(QTreeWidgetItem* parent, const std::filesystem::path& file);

	QString getFilename() const;

	bool hasExperimentDocument() const;
	nlohmann::json getExperimentDocument() const;

private:
	std::filesystem::path mExperimentFile;
};

