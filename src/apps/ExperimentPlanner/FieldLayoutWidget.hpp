
#pragma once

#include <QWidget>

#include "Spidercam/SpidercamScanArea.hpp"
#include "ExperimentTypes.hpp"

#include <spidercam/spidercam_types.hpp>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
//class QAbstractButton;
//class QPushButton;
//class QLineEdit;
//class QCheckBox;
//class QLabel;
class QStatusBar;
QT_END_NAMESPACE


class cFieldLayoutWidget : public QWidget
{
	Q_OBJECT

public:
	cFieldLayoutWidget(QWidget* parent = nullptr);

	void initialize();

	bool isDirty() const;

	void setBounds(double minX_mm, double maxX_mm, double minY_mm, double maxY_mm);

	void addLayout(const cSpidercamScanArea::experimentLayout_t& layout);
	void replaceLayout(const cSpidercamScanArea::experimentLayout_t& original_layout, const cSpidercamScanArea::experimentLayout_t& new_layout);

	cSpidercamScanArea::experimentLayout_t findLayout(const QString& label);

	const std::vector<cSpidercamScanArea::experimentLayout_t>& getLayouts() const;

	void load(const QString& layout_filename);
	void save(const QString& layout_filename);

public slots:
	void onConnectToSpidercam();
	void onDisconnectFromSpidercam();

public slots:
	void updateLimits(spidercam::sWorkingDimensions limits);
	void updatePosition(spidercam::sPosition_1_t pos);

public slots:
	void updateRecordingState(bool recording);
	void clearRecordingPath();
	void drawRecordingPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);

public slots:
	void refresh();
	void experimentStateChanging(experiment::eState state);
	void experimentStatusUpdating(QString msg);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

private:
	bool mDirty = false;

	cSpidercamScanArea* mpScanArea = nullptr;

	QStatusBar* mpExperimentStatus = nullptr;
};
