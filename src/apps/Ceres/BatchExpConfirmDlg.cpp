
#include "BatchExpConfirmDlg.hpp"

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

/*
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
*/

void cBatchExpConfirmDlg::accept()
{
    QDialog::accept();
}

void cBatchExpConfirmDlg::reject()
{
    QDialog::reject();
}


