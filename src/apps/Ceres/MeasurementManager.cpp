
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

    //    SingleSelection,
    //    MultiSelection,
    //    ExtendedSelection,
    //    ContiguousSelection

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    if (!fs::exists(mMeasurementPath))
    {
        fs::create_directory(mMeasurementPath);
    }

    loadMeasurements();
}

void cMeasurementManager::refresh()
{
    clear();
    loadMeasurements();
}

void cMeasurementManager::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu contextMenu(this);

    QAction run("Run...", this);
    connect(&run, &QAction::triggered, this, &cMeasurementManager::runExperiments);
    contextMenu.addAction(&run);

    contextMenu.exec(event->globalPos());
}


const cMeasurementTreeItem* cMeasurementManager::measurements() const
{
    return nullptr;
}

void cMeasurementManager::loadMeasurements()
{
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
                // Check for acceptable file extensions
                auto extension = entry.path().extension().string();

                if (extension == ".json")
                    auto* pItem = new cMeasurementTreeItem(this, entry.path());
            }
            catch (const invalid_experiment_file& e)
            {
            }
            catch (const std::exception& e)
            {
            }
        }
    }
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
                // Check for acceptable file extensions
                auto extension = entry.path().extension().string();

                if (extension == ".json")
                    auto* pItem = new cMeasurementTreeItem(&root, entry.path());
            }
            catch (const invalid_experiment_file&)
            {
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

                QMessageBox mb(QMessageBox::Critical, "Measurement File Error", msg);
                mb.exec();
            }
            catch (const std::exception& e)
            {
                QString msg = "Unknown error in ";
                msg += entry.path().filename().string().c_str();
                msg += ".\n";
                msg += e.what();

                QMessageBox mb(QMessageBox::Critical, "Measurement File Error", msg);
                mb.exec();
            }
        }
    }
}

/*
 *  Selections Dialog
 */
/*
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

//    SingleSelection,
//    MultiSelection,
//    ExtendedSelection,
//    ContiguousSelection

    mpMeasurements->setSelectionMode(QAbstractItemView::ExtendedSelection);

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
*/

