
#pragma once

#include <QtWidgets>
#include <QDialog>

#include <vector>
#include <filesystem>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

// Forward Declarations
class cMeasurementTreeItem;

class cMeasurementManager : public QTreeWidget
{
	Q_OBJECT

public:
	cMeasurementManager(const QString& path, QWidget* parent = nullptr);

	const cMeasurementTreeItem* measurements() const;

signals:
	void runExperiments();

public slots:
	void refresh();

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

private:
	void loadMeasurements();
	void loadMeasurements(cMeasurementTreeItem& parent, const std::filesystem::path& path);

private:
	cMeasurementTreeItem*	mpMeasurementItems;
	std::filesystem::path	mMeasurementPath;
};

class cMeasurementSelectDlg : public QDialog
{
	Q_OBJECT

public:
	cMeasurementSelectDlg(QWidget* parent = nullptr);
	~cMeasurementSelectDlg();

	void initialize(const cMeasurementManager& mgr);

	cMeasurementTreeItem* currentItem() const;

private slots:
	void accept() override;
	void reject() override;

private:
	QTreeWidget* mpMeasurements;
};
