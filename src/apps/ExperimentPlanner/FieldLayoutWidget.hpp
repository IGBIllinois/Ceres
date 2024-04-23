
#pragma once

#include <QWidget>

#include "Spidercam/SpidercamScanArea.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
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

	const std::vector<cSpidercamScanArea::experimentLayout_t>& getLayouts() const;

	void load(const QString& layout_filename);
	void save(const QString& layout_filename);

public slots:
	void clearRecordingPath();
	void drawRecordingPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

private:
	bool mDirty = false;

	cSpidercamScanArea* mpScanArea = nullptr;

};
