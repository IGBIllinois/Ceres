
#include "MeasurementManager.hpp"
#include "MeasurementTreeItem.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMenu>

#include <string>

namespace fs = std::filesystem;

cMeasurementManager::cMeasurementManager(const QString& path, QWidget* parent)
	: QTreeWidget(parent),
    mpMeasurementItems(nullptr),
    mMeasurementPath(path.toStdString())
{
    clear();
    setColumnCount(1);
    setHeaderLabel("Loaded Measurements");
    setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    if (!path.isEmpty())
    {
        if (!fs::exists(mMeasurementPath))
        {
            fs::create_directory(mMeasurementPath);
        }
    }

    loadMeasurements();

    connect(this, &QTreeWidget::itemDoubleClicked, this, &cMeasurementManager::onItemDoubleClick);
}

void cMeasurementManager::onConnectToSpidercam()
{
    mConnected = true;
}

void cMeasurementManager::onDisconnectFromSpidercam()
{
    mConnected = false;
}

void cMeasurementManager::reloadMeasurements()
{
    clear();
    loadMeasurements();
}

void cMeasurementManager::reloadMeasurements(QString path)
{
    mMeasurementPath = path.toStdString();
    clear();
    loadMeasurements();
}

void cMeasurementManager::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu contextMenu(this);

    QAction* menuItem = new QAction("Open...", this);
    connect(menuItem, &QAction::triggered, this, &cMeasurementManager::openMeasurement);
    contextMenu.addAction(menuItem);

    if (mConnected)
    {
        contextMenu.addSeparator();

        QAction* menuItem = new QAction("Test...", this);
        connect(menuItem, &QAction::triggered, this, &cMeasurementManager::testMeasurement);
        contextMenu.addAction(menuItem);
    }

    contextMenu.exec(event->globalPos());
}

void cMeasurementManager::openMeasurement()
{
    auto pItem = dynamic_cast<cMeasurementTreeItem*>(currentItem());
    if (!pItem)
        return;

    auto path = pItem->getMeasurementFile();

    QString filename = QString::fromStdString(path.string());

    emit loadMeasurement(filename);
}

void cMeasurementManager::testMeasurement()
{
    auto pItem = dynamic_cast<cMeasurementTreeItem*>(currentItem());
    if (!pItem)
        return;

    auto path = pItem->getMeasurementFile();

    QString filename = QString::fromStdString(path.string());

    emit runMeasurement(filename);
}

void cMeasurementManager::onItemDoubleClick(QTreeWidgetItem* item, int column)
{
    if (item)
    {
        auto pItem = dynamic_cast<cMeasurementTreeItem*>(item);
        if (!pItem)
            return;

        auto path = pItem->getMeasurementFile();

        if (path.empty())
        {
            expandItem(item);
        }
        else
        {
            QString filename = QString::fromStdString(path.string());

            emit loadMeasurement(filename);
        }
    }
}

const cMeasurementTreeItem* cMeasurementManager::measurements() const
{
    return nullptr;
}

void cMeasurementManager::loadMeasurements()
{
    if (mMeasurementPath.string().empty())
        return;

//    mpExperimentItems = new cExperimentTreeItem(this, "Experiments");

//    loadExperiments(*mpExperimentItems, mExperimentPath);
    for (auto entry : fs::directory_iterator(mMeasurementPath))
    {
        if (entry.is_directory())
        {
            QString name = entry.path().filename().string().c_str();
            cMeasurementTreeItem* level = new cMeasurementTreeItem(this, name);
            loadMeasurements(*level, entry.path());
        }

        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cMeasurementTreeItem(this, entry.path());
            }
            catch (const std::exception& e)
            {
            }
        }
    }

//    addTopLevelItem(mpExperimentItems);
}

void cMeasurementManager::loadMeasurements(cMeasurementTreeItem& root, const std::filesystem::path& path)
{
    for (auto entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            QString name = entry.path().filename().string().c_str();
            cMeasurementTreeItem* level = new cMeasurementTreeItem(&root, name);
            loadMeasurements(*level, entry.path());
        }

        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cMeasurementTreeItem(&root, entry.path());
            }
            catch (const std::invalid_argument&)
            {
            }
            catch (const nlohmann::json::parse_error& e)
            {
                QString msg = "Parsing error in ";
                msg += entry.path().filename().string().c_str();
                msg += ".\n";
                msg += e.what();

                QMessageBox mb(QMessageBox::Critical, "Measure File Error", msg);
                mb.exec();
            }
            catch (const std::exception& e)
            {
                QString msg = "Unknown error in ";
                msg += entry.path().filename().string().c_str();
                msg += ".\n";
                msg += e.what();

                QMessageBox mb(QMessageBox::Critical, "Measure File Error", msg);
                mb.exec();
            }
        }
    }
}

/*
 *  Selections Dialog
 */
cMeasurementSelectDlg::cMeasurementSelectDlg(QWidget* parent)
:
    QDialog(parent),
    mpMeasurements(nullptr)
{
    setWindowTitle(tr("Select Measurement"));
}

cMeasurementSelectDlg::~cMeasurementSelectDlg()
{}

void cMeasurementSelectDlg::initialize(const cMeasurementManager& mgr)
{
    mpMeasurements = new QTreeWidget(this);
    mpMeasurements->setColumnCount(1);
    mpMeasurements->setHeaderLabel("Loaded Measurements");
    mpMeasurements->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    mpMeasurements->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpMeasurements->setSelectionMode(QAbstractItemView::SingleSelection);
    auto* active = mgr.measurements();
    auto n = active->childCount();

    QList<QTreeWidgetItem*> items;
    for (int i = 0; i < n; ++i)
    {
        auto* pChild = static_cast<cMeasurementTreeItem*>(active->child(i));
        auto* item = new cMeasurementTreeItem(mpMeasurements, pChild->text(0), pChild->getMeasurementFile());
        items.append(item);
    }

    mpMeasurements->insertTopLevelItems(0, items);

    QVBoxLayout* pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(mpMeasurements);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    pMainLayout->addWidget(buttonBox);

    setLayout(pMainLayout);
}

cMeasurementTreeItem* cMeasurementSelectDlg::currentItem() const
{
    if (mpMeasurements)
        return static_cast<cMeasurementTreeItem*>(mpMeasurements->currentItem());

    return nullptr;
}

void cMeasurementSelectDlg::accept()
{
    QDialog::accept();
}

void cMeasurementSelectDlg::reject()
{
    QDialog::reject();
}


