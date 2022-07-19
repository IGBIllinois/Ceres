
#include "BatchExpConfirmDlg.hpp"
#include "ExperimentTreeItem.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>

#include <string>

namespace fs = std::filesystem;

/*
 *  Selections Dialog
 */
cBatchExpConfirmDlg::cBatchExpConfirmDlg(QWidget* parent)
:
    QDialog(parent)
{
    setWindowTitle(tr("Select Experiment"));
}

cBatchExpConfirmDlg::~cBatchExpConfirmDlg()
{}

void cBatchExpConfirmDlg::initialize(const cExperimentTreeItem* pRoot)
{
    mpExperiments = new QTreeWidget(this);
    mpExperiments->setColumnCount(0);
    mpExperiments->setHeaderLabel("Experiments to run...");
    mpExperiments->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    mpExperiments->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto n = pRoot->childCount();
    for (int i = 0; i < n; ++i)
    {
        auto* pExp = static_cast<cExperimentTreeItem*>(pRoot->child(i));
        if (pExp->hasExperimentDocument())
        {
            auto* item = new cExperimentTreeItem(mpExperiments, pExp->text(0), pExp->getExperimentFile());
            item->setCheckState(0, Qt::Checked);
        }
        else
        {
            auto* item = new cExperimentTreeItem(mpExperiments, pExp->text(0));
            loadExperiments(item, pExp);
            item->setCheckState(0, Qt::Checked);
        }
    }

    connect(mpExperiments, &QTreeWidget::itemChanged, this, &cBatchExpConfirmDlg::itemChanged);

    QVBoxLayout* pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(mpExperiments);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    pMainLayout->addWidget(buttonBox);

    setLayout(pMainLayout);
}

void cBatchExpConfirmDlg::loadExperiments(cExperimentTreeItem* pRoot,
                                            const cExperimentTreeItem* pBranch)
{
    auto n = pBranch->childCount();
    for (int i = 0; i < n; ++i)
    {
        auto* pExp = static_cast<cExperimentTreeItem*>(pBranch->child(i));
        if (pExp->hasExperimentDocument())
        {
            auto* item = new cExperimentTreeItem(pRoot, pExp->text(0), pExp->getExperimentFile());
            item->setCheckState(0, Qt::Checked);
        }
        else
        {
            auto* item = new cExperimentTreeItem(mpExperiments, pExp->text(0));
            loadExperiments(item, pExp);
            item->setCheckState(0, Qt::Checked);
        }
    }

}


std::vector<std::filesystem::path> cBatchExpConfirmDlg::getSelectedExperiments()
{
    std::vector<std::filesystem::path> selected;

    auto n = mpExperiments->topLevelItemCount();
    for (int i = 0; i < n; ++i)
    {
        //    auto* item = static_cast<cExperimentTreeItem>(mpExperiments->topLevelItem(0));
        auto* item = mpExperiments->topLevelItem(i);
        cExperimentTreeItem* pExp = static_cast<cExperimentTreeItem*>(item);
        if (pExp->hasExperimentDocument())
        {
            if (item->checkState(0) == Qt::Checked)
            {
                selected.push_back(pExp->getExperimentFile());
            }
        }
        else
        {
            auto tmp = getSelectedExperiments(pExp);
            selected.insert(selected.end(), tmp.begin(), tmp.end());
        }
    }

    return selected;
}

std::vector<std::filesystem::path> cBatchExpConfirmDlg::getSelectedExperiments(cExperimentTreeItem* pRoot)
{
    std::vector<std::filesystem::path> selected;

    auto n = pRoot->childCount();
    for (int i = 0; i < n; ++i)
    {
        //    auto* item = static_cast<cExperimentTreeItem>(mpExperiments->topLevelItem(0));
        auto* item = pRoot->child(i);
        cExperimentTreeItem* pExp = static_cast<cExperimentTreeItem*>(item);
        if (pExp->hasExperimentDocument())
        {
            if (item->checkState(0) == Qt::Checked)
            {
                selected.push_back(pExp->getExperimentFile());
            }
        }
        else
        {
            auto tmp = getSelectedExperiments(pExp);
            selected.insert(selected.end(), tmp.begin(), tmp.end());
        }
    }

    return selected;
}


void cBatchExpConfirmDlg::accept()
{
    QDialog::accept();
}

void cBatchExpConfirmDlg::reject()
{
    QDialog::reject();
}

void cBatchExpConfirmDlg::itemChanged(QTreeWidgetItem* item, int column)
{
    auto* pExp = static_cast<cExperimentTreeItem*>(item);

    if (pExp->hasExperimentDocument())
    {
        return;
    }

    auto n = item->childCount();
    for (int i = 0; i < n; ++i)
    {
        auto* child = item->child(i);
        child->setCheckState(column, item->checkState(column));
    }
}


