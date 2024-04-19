
#pragma once

#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE

// Forward Declarations
class cSpidercamScanArea;


class cFieldLayoutWidget : public QWidget
{
	Q_OBJECT

public:
	cFieldLayoutWidget(QWidget* parent = nullptr);

	void initialize();

	void setBounds(double minX_mm, double maxX_mm, double minY_mm, double maxY_mm);

	void load(const QString& layout_filename);
	void save(const QString& layout_filename);

public slots:
	void clearRecordingPath();
	void drawRecordingPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

private:
	cSpidercamScanArea* mpScanArea = nullptr;

};
