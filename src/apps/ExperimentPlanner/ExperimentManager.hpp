
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
	cExperimentManager(const QString& path, QWidget* parent = nullptr);

	const cExperimentTreeItem* experiments() const;

signals:
	void loadExperiment(const QString& file_name);

public slots:
	void reloadExperiments(QString path);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
	void openExperiment();

private:
	void loadExperiments();
	void loadExperiments(cExperimentTreeItem& parent, const std::filesystem::path& path);

private:
	cExperimentTreeItem*	mpExperimentItems;
	std::filesystem::path	mExperimentPath;
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
