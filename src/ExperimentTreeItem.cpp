
#include "ExperimentTreeItem.hpp"

#include <fstream>


cExperimentTreeItem::cExperimentTreeItem(QTreeWidget* parent, const QString& text)
	: QTreeWidgetItem(parent)
{
	setText(0, text);
}

cExperimentTreeItem::cExperimentTreeItem(QTreeWidget* parent, const std::filesystem::path& experiment_file)
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

cExperimentTreeItem::cExperimentTreeItem(QTreeWidget* parent, const QString& text, const std::filesystem::path& experiment_file)
    : QTreeWidgetItem(parent)
{
    setText(0, text);
    mExperimentFile = experiment_file;
}

cExperimentTreeItem::cExperimentTreeItem(QTreeWidgetItem* parent, const QString& text)
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

cExperimentTreeItem::cExperimentTreeItem(QTreeWidgetItem* parent, const QString& name, const std::filesystem::path& experiment_file)
    : QTreeWidgetItem(parent)
{
    setText(0, name);
    mExperimentFile = experiment_file;
}

cExperimentTreeItem::cExperimentTreeItem(const cExperimentTreeItem& other)
    : QTreeWidgetItem(other)
{
    mExperimentFile = other.mExperimentFile;
}


QString cExperimentTreeItem::getFilename() const
{
    QString filename = mExperimentFile.string().c_str();
    return filename;
}

const std::filesystem::path& cExperimentTreeItem::getExperimentFile() const
{
    return mExperimentFile;
}

bool cExperimentTreeItem::hasExperimentDocument() const
{
    return !mExperimentFile.empty();
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
