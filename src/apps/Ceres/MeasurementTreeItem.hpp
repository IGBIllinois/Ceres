
#pragma once

#include <QtWidgets>
#include <QString>

#include <nlohmann/json.hpp>
#include <filesystem>

class cExperimentTreeItem : public QTreeWidgetItem
{
public:
	cExperimentTreeItem(QTreeWidget* parent, const QString& text);
	cExperimentTreeItem(QTreeWidget* parent, const std::filesystem::path& file);
	cExperimentTreeItem(QTreeWidget* parent, const QString& name, const std::filesystem::path& file);

	cExperimentTreeItem(QTreeWidgetItem* parent, const QString& text);
	cExperimentTreeItem(QTreeWidgetItem* parent, const std::filesystem::path& file);
	cExperimentTreeItem(QTreeWidgetItem* parent, const QString& name, const std::filesystem::path& file);

	cExperimentTreeItem(const cExperimentTreeItem& other);

	QString getFilename() const;
	const std::filesystem::path& getExperimentFile() const;

	bool hasExperimentDocument() const;
	nlohmann::json getExperimentDocument() const;

private:
	std::filesystem::path mExperimentFile;
};

