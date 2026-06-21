
#include "MeasurementTreeItem.hpp"

#include <fstream>


cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidget* parent, const QString& text)
	: QTreeWidgetItem(parent)
{
	setText(0, text);
}

cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidget* parent, const std::filesystem::path& measurement_file)
    : QTreeWidgetItem(parent)
{
    std::ifstream in;
    in.open(measurement_file);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc = nlohmann::json::parse(in, nullptr, false, true);

    if (!jsonDoc.contains("experiment"))
    {
        in.close();
        throw invalid_experiment_file();
    }

    std::string measurement_name;

    if (jsonDoc.contains("measurement name"))
        measurement_name = jsonDoc["measurement name"];

    else if (jsonDoc.contains("measurement_name"))
        measurement_name = jsonDoc["measurement_name"];

    else if (jsonDoc.contains("experiment name"))
        measurement_name = jsonDoc["experiment name"];

    else if (jsonDoc.contains("experiment_name"))
        measurement_name = jsonDoc["experiment_name"];

    if (measurement_name.empty())
    {
        in.close();
        throw invalid_experiment_file();
    }

    QString name = QString::fromStdString(measurement_name);
    setText(0, name);

    mMeasurementFile = measurement_file;
}

cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidget* parent, const QString& text, const std::filesystem::path& measurement_file)
    : QTreeWidgetItem(parent)
{
    setText(0, text);
    mMeasurementFile = measurement_file;
}

cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidgetItem* parent, const QString& text)
    : QTreeWidgetItem(parent)
{
    setText(0, text);
}

cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidgetItem* parent, const std::filesystem::path& measurement_file)
	: QTreeWidgetItem(parent)
{
    std::ifstream in;
    in.open(measurement_file);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc = nlohmann::json::parse(in, nullptr, true, true);

    if (!jsonDoc.contains("experiment"))
    {
        in.close();
        throw invalid_experiment_file();
    }

    std::string measurement_name;

    if (jsonDoc.contains("measurement name"))
        measurement_name = jsonDoc["measurement name"];

    else if (jsonDoc.contains("measurement_name"))
        measurement_name = jsonDoc["measurement_name"];

    else if (jsonDoc.contains("experiment name"))
        measurement_name = jsonDoc["experiment name"];
    else if (jsonDoc.contains("experiment_name"))
        measurement_name = jsonDoc["experiment_name"];
    else
    {
        in.close();
        throw invalid_experiment_file();
    }

    QString name = QString::fromStdString(measurement_name);
    setText(0, name);

    mMeasurementFile = measurement_file;
}

cMeasurementTreeItem::cMeasurementTreeItem(QTreeWidgetItem* parent, const QString& name, const std::filesystem::path& measurement_file)
    : QTreeWidgetItem(parent)
{
    setText(0, name);
    mMeasurementFile = measurement_file;
}

cMeasurementTreeItem::cMeasurementTreeItem(const cMeasurementTreeItem& other)
    : QTreeWidgetItem(other)
{
    mMeasurementFile = other.mMeasurementFile;
}


QString cMeasurementTreeItem::getFilename() const
{
    QString filename = mMeasurementFile.string().c_str();
    return filename;
}

const std::filesystem::path& cMeasurementTreeItem::getMeasurementFile() const
{
    return mMeasurementFile;
}

bool cMeasurementTreeItem::hasMeasurementDocument() const
{
    return !mMeasurementFile.empty();
}

nlohmann::json cMeasurementTreeItem::getMeasurementDocument() const
{
    std::ifstream in;
    in.open(mMeasurementFile);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc = nlohmann::json::parse(in, nullptr, false, true);

    return jsonDoc;
}
