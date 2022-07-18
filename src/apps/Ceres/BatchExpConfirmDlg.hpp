
#pragma once

#include <QtWidgets>
#include <QDialog>
#include <filesystem>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
class QLabel;
QT_END_NAMESPACE

// Forward Declaration
class cExperimentTreeItem;


class cBatchExpConfirmDlg : public QDialog
{
	Q_OBJECT

public:
	cBatchExpConfirmDlg(QWidget* parent = nullptr);
	~cBatchExpConfirmDlg();

	void initialize(const cExperimentTreeItem* pRoot);

	std::vector<std::filesystem::path> getSelectedExperiments();

private slots:
	void accept() override;
	void reject() override;

	void itemChanged(QTreeWidgetItem* item, int column);

private:
	void loadExperiments(cExperimentTreeItem* pRoot, 
						 const cExperimentTreeItem* pBranch);

	std::vector<std::filesystem::path> getSelectedExperiments(cExperimentTreeItem* pRoot);

private:
	QTreeWidget* mpExperiments;
};
