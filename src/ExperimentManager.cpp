
#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>

#include <string>

namespace fs = std::filesystem;

cExperimentManager::cExperimentManager(QWidget* parent)
	: QTreeWidget(parent),
    mpActiveItems(nullptr),
    mpArchiveItems(nullptr)
{
    clear();
    setColumnCount(1);
    setHeaderLabel("Loaded Experiments");
    setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    auto cwd = std::filesystem::current_path();

    mActivePath = cwd / "Active";
    mArchivePath = cwd / "Archive";

    if (!fs::exists(mActivePath))
    {
        fs::create_directory(mActivePath);
    }

    if (!fs::exists(mArchivePath))
    {
        fs::create_directory(mArchivePath);
    }

    loadActiveExperiments();
    loadArchivedExperiments();
}

const cExperimentTreeItem* cExperimentManager::activeItems() const
{
    return mpActiveItems;
}

const cExperimentTreeItem* cExperimentManager::archiveItems() const
{
    return mpArchiveItems;
}

void cExperimentManager::loadActiveExperiments()
{
    mpActiveItems = new cExperimentTreeItem(this, "Active");

    for (auto entry : fs::directory_iterator(mActivePath))
    {
        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cExperimentTreeItem(mpActiveItems, entry.path());
            }
            catch(const std::exception& e)
            { }
        }
    }

    addTopLevelItem(mpActiveItems);
}

void cExperimentManager::loadArchivedExperiments()
{
    mpArchiveItems = new cExperimentTreeItem(this, "Archive");
    addTopLevelItem(mpArchiveItems);
}

/*
 *  Selections Dialog
 */
cExperimentSelectDlg::cExperimentSelectDlg(QWidget* parent)
:
    QDialog(parent),
    mpExperiments(nullptr)
{
    setWindowTitle(tr("Select Experiment"));
}

cExperimentSelectDlg::~cExperimentSelectDlg()
{}

void cExperimentSelectDlg::initialize(const cExperimentManager& mgr)
{
    mpExperiments = new QTreeWidget(this);
    mpExperiments->setColumnCount(1);
    mpExperiments->setHeaderLabel("Loaded Experiments");
    mpExperiments->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    mpExperiments->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpExperiments->setSelectionMode(QAbstractItemView::SingleSelection);
    auto* active = mgr.activeItems();
    auto n = active->childCount();

    QList<QTreeWidgetItem*> items;
    for (int i = 0; i < n; ++i)
    {
        auto* pChild = static_cast<cExperimentTreeItem*>(active->child(i));
        auto* item = new cExperimentTreeItem(mpExperiments, pChild->text(0), pChild->getExperimentFile());
        items.append(item);
    }

    mpExperiments->insertTopLevelItems(0, items);

    QVBoxLayout* pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(mpExperiments);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    pMainLayout->addWidget(buttonBox);

    setLayout(pMainLayout);
}

cExperimentTreeItem* cExperimentSelectDlg::currentItem() const
{
    if (mpExperiments)
        return static_cast<cExperimentTreeItem*>(mpExperiments->currentItem());

    return nullptr;
}

void cExperimentSelectDlg::accept()
{
    QDialog::accept();
}

void cExperimentSelectDlg::reject()
{
    QDialog::reject();
}


