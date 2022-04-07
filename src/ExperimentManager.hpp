
#pragma once

#include <QtWidgets>
#include <QDialog>
#include <filesystem>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

// Forward Declarations
class cExperimentTreeItem;

class cExperimentManager : public QTreeWidget
{
	Q_OBJECT

public:
	cExperimentManager(QWidget* parent = nullptr);

	const cExperimentTreeItem* activeItems() const;
	const cExperimentTreeItem* archiveItems() const;

private:
	void loadActiveExperiments();
	void loadArchivedExperiments();

private:
	cExperimentTreeItem*	mpActiveItems;
	cExperimentTreeItem*	mpArchiveItems;
	std::filesystem::path	mActivePath;
	std::filesystem::path	mArchivePath;
};

class cExperimentSelectDlg : public QDialog
{
	Q_OBJECT

public:
	cExperimentSelectDlg(QWidget* parent = nullptr);
	~cExperimentSelectDlg();

	void initialize(const cExperimentManager& mgr);

	cExperimentTreeItem* currentItem() const;

private slots:
	void accept() override;
	void reject() override;

private:
	QTreeWidget* mpExperiments;
};
