
#include "BatchExpConfirmDlg.hpp"
#include "MeasurementTreeItem.hpp"

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
    setWindowTitle(tr("Select Measurement"));
}

cBatchExpConfirmDlg::~cBatchExpConfirmDlg()
{}

void cBatchExpConfirmDlg::initialize(const cMeasurementTreeItem* pRoot)
{
    mpMeasurements = new QTreeWidget(this);
    mpMeasurements->setColumnCount(0);
    mpMeasurements->setHeaderLabel("Measurements to run...");
    mpMeasurements->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    mpMeasurements->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto n = pRoot->childCount();
    for (int i = 0; i < n; ++i)
    {
        auto* pExp = static_cast<cMeasurementTreeItem*>(pRoot->child(i));
        if (pExp->hasMeasurementDocument())
        {
            auto* item = new cMeasurementTreeItem(mpMeasurements, pExp->text(0), pExp->getMeasurementFile());
            item->setCheckState(0, Qt::Checked);
        }
        else
        {
            auto m = pExp->childCount();
            auto* item = new cMeasurementTreeItem(mpMeasurements, pExp->text(0));
            loadExperiments(item, pExp);
            item->setCheckState(0, Qt::Checked);
        }
    }

    connect(mpMeasurements, &QTreeWidget::itemChanged, this, &cBatchExpConfirmDlg::itemChanged);

    QVBoxLayout* pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(mpMeasurements);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    pMainLayout->addWidget(buttonBox);

    setLayout(pMainLayout);
}

void cBatchExpConfirmDlg::loadExperiments(cMeasurementTreeItem* pRoot,
                                            cMeasurementTreeItem* pBranch)
{
    auto n = pBranch->childCount();
    for (int i = 0; i < n; ++i)
    {
        auto* pExp = static_cast<cMeasurementTreeItem*>(pBranch->child(i));
        if (pExp->hasMeasurementDocument())
        {
            auto* item = new cMeasurementTreeItem(pRoot, pExp->text(0), pExp->getMeasurementFile());
            item->setCheckState(0, Qt::Checked);
        }
        else
        {
            auto* item = new cMeasurementTreeItem(pRoot, pExp->text(0));
            loadExperiments(item, pExp);
            item->setCheckState(0, Qt::Checked);
        }
    }

}


std::vector<std::filesystem::path> cBatchExpConfirmDlg::getSelectedMeasurements()
{
    std::vector<std::filesystem::path> selected;

    auto n = mpMeasurements->topLevelItemCount();
    for (int i = 0; i < n; ++i)
    {
        //    auto* item = static_cast<cExperimentTreeItem>(mpExperiments->topLevelItem(0));
        auto* item = mpMeasurements->topLevelItem(i);
        cMeasurementTreeItem* pExp = static_cast<cMeasurementTreeItem*>(item);
        if (pExp->hasMeasurementDocument())
        {
            if (item->checkState(0) == Qt::Checked)
            {
                selected.push_back(pExp->getMeasurementFile());
            }
        }
        else
        {
            auto tmp = getSelectedMeasurements(pExp);
            selected.insert(selected.end(), tmp.begin(), tmp.end());
        }
    }

    return selected;
}

std::vector<std::filesystem::path> cBatchExpConfirmDlg::getSelectedMeasurements(cMeasurementTreeItem* pRoot)
{
    std::vector<std::filesystem::path> selected;

    auto n = pRoot->childCount();
    for (int i = 0; i < n; ++i)
    {
        //    auto* item = static_cast<cExperimentTreeItem>(mpExperiments->topLevelItem(0));
        auto* item = pRoot->child(i);
        cMeasurementTreeItem* pExp = static_cast<cMeasurementTreeItem*>(item);
        if (pExp->hasMeasurementDocument())
        {
            if (item->checkState(0) == Qt::Checked)
            {
                selected.push_back(pExp->getMeasurementFile());
            }
        }
        else
        {
            auto tmp = getSelectedMeasurements(pExp);
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
    auto* pExp = static_cast<cMeasurementTreeItem*>(item);

    if (pExp->hasMeasurementDocument())
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


