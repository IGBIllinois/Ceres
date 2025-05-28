
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
class cMeasurementTreeItem;


class cBatchExpConfirmDlg : public QDialog
{
	Q_OBJECT

public:
	cBatchExpConfirmDlg(QWidget* parent = nullptr);
	~cBatchExpConfirmDlg();

	void initialize(const cMeasurementTreeItem* pRoot);

	std::vector<std::filesystem::path> getSelectedMeasurements();

private slots:
	void accept() override;
	void reject() override;

	void itemChanged(QTreeWidgetItem* item, int column);

private:
	void loadExperiments(cMeasurementTreeItem* pRoot,
						 const cMeasurementTreeItem* pBranch);

	std::vector<std::filesystem::path> getSelectedMeasurements(cMeasurementTreeItem* pRoot);

private:
	QTreeWidget* mpMeasurements;
};
