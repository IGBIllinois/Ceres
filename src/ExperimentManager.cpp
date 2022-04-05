
#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"

#include <string>

namespace fs = std::filesystem;

cExperimentManager::cExperimentManager(QWidget* parent)
	: QTreeWidget(parent)
{
    clear();
    setColumnCount(1);
    setHeaderLabel("Loaded Experiments");

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

void cExperimentManager::loadActiveExperiments()
{
    cExperimentTreeItem* pTopItem = new cExperimentTreeItem(this, "Active");

    for (auto entry : fs::directory_iterator(mActivePath))
    {
        if (entry.is_regular_file())
        {
            try
            {
                auto* pItem = new cExperimentTreeItem(pTopItem, entry.path());
            }
            catch(const std::exception& e)
            { }
        }
    }

//    cExperimentTreeItem* pItem1 = new cExperimentTreeItem(pTopItem, "Item1");
//    cExperimentTreeItem* pItem2 = new cExperimentTreeItem(pTopItem, "Item2");
//    cExperimentTreeItem* pItem3 = new cExperimentTreeItem(pTopItem, "Item3");
    addTopLevelItem(pTopItem);
}

void cExperimentManager::loadArchivedExperiments()
{
    cExperimentTreeItem* pTopItem = new cExperimentTreeItem(this, "Archive");
    addTopLevelItem(pTopItem);
}

/*
 *  Selections Dialog
 */
cExperimentSelectDlg::cExperimentSelectDlg(QWidget* parent)
:
    QDialog(parent)
{}



