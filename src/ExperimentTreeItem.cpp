
#include "ExperimentTreeItem.hpp"

#include <fstream>


cExperimentTreeItem::cExperimentTreeItem(QTreeWidget* parent, const QString& text)
	: QTreeWidgetItem(parent)
{
	setText(0, text);
}

cExperimentTreeItem::cExperimentTreeItem(QTreeWidgetItem* parent, const std::filesystem::path& experiment_file)
	: QTreeWidgetItem(parent)
{
    std::ifstream in;
    in.open(experiment_file);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc;
    in >> jsonDoc;

    QString name = static_cast<std::string>(jsonDoc["experiment_name"]).c_str();
    setText(0, name);

    mExperimentFile = experiment_file;
}

nlohmann::json cExperimentTreeItem::getExperimentDocument() const
{
    std::ifstream in;
    in.open(mExperimentFile);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc;
    in >> jsonDoc;

    return jsonDoc;
}
