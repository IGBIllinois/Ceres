
#pragma once

#include <QTreeWidgetItem>
#include <QString>

#include <nlohmann/json.hpp>
#include <filesystem>

class cMeasurementTreeItem : public QTreeWidgetItem
{
public:
	cMeasurementTreeItem(QTreeWidget* parent, const QString& text);
	cMeasurementTreeItem(QTreeWidget* parent, const std::filesystem::path& file);
	cMeasurementTreeItem(QTreeWidget* parent, const QString& name, const std::filesystem::path& file);

	cMeasurementTreeItem(QTreeWidgetItem* parent, const QString& text);
	cMeasurementTreeItem(QTreeWidgetItem* parent, const std::filesystem::path& file);
	cMeasurementTreeItem(QTreeWidgetItem* parent, const QString& name, const std::filesystem::path& file);

	cMeasurementTreeItem(const cMeasurementTreeItem& other);

	QString getFilename() const;
	const std::filesystem::path& getMeasurementFile() const;

	bool hasMeasurementDocument() const;
	nlohmann::json getMeasurementDocument() const;

private:
	std::filesystem::path mMeasurementFile;
};

