
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

cExperimentManager::cExperimentManager(const QString& path, QWidget* parent)
	: QTreeWidget(parent),
    mpExperimentItems(nullptr),
    mExperimentPath(path.toStdString())
{
    clear();
    setColumnCount(1);
    setHeaderLabel("Loaded Experiments");
    setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    if (!fs::exists(mExperimentPath))
    {
        fs::create_directory(mExperimentPath);
    }

    loadExperiments();
}

void cExperimentManager::refresh()
{
    clear();
    loadExperiments();
}

const cExperimentTreeItem* cExperimentManager::experiments() const
{
    return nullptr;
}

void cExperimentManager::loadExperiments()
{
//    mpExperimentItems = new cExperimentTreeItem(this, "Experiments");

//    loadExperiments(*mpExperimentItems, mExperimentPath);
    for (auto entry : fs::directory_iterator(mExperimentPath))
    {
        if (entry.is_directory())
        {
            QString name = entry.path().filename().string().c_str();
            cExperimentTreeItem* level = new cExperimentTreeItem(this, name);
            loadExperiments(*level, entry.path());
        }

        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cExperimentTreeItem(this, entry.path());
            }
            catch (const std::exception& e)
            {
            }
        }
    }

//    addTopLevelItem(mpExperimentItems);
}

void cExperimentManager::loadExperiments(cExperimentTreeItem& root, const std::filesystem::path& path)
{
    for (auto entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            QString name = entry.path().filename().string().c_str();
            cExperimentTreeItem* level = new cExperimentTreeItem(&root, name);
            loadExperiments(*level, entry.path());
        }

        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cExperimentTreeItem(&root, entry.path());
            }
            catch (const std::exception& e)
            {
                std::string msg = e.what();
            }
        }
    }
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
    auto* active = mgr.experiments();
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


