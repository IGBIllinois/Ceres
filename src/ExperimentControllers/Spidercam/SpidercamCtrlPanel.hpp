#pragma once


#include <spidercam_connect/spidercam_types.hpp>

#include <QAbstractScrollArea>
#include <QModelIndex>

// Forward Declaration
class cViewToolbar;


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QComboBox;
class QVBoxLayout;
class QTableView;
class QStandardItem;
class QStandardItemModel;
class QStackedLayout;
class QSplitter;
QT_END_NAMESPACE


class cSpidercamCtrlPanel : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cSpidercamCtrlPanel(QWidget* parent = nullptr);
	virtual ~cSpidercamCtrlPanel();

signals:
	void stepX(int x_mm);
	void stepY(int y_mm);
	void stepZ(int z_mm);

public slots:
	void updateLimits(spidercam::sWorkingDimensions limits);
	void updatePosition(spidercam::sPosition_1_t pos);

protected slots:
	void onMoveEast();
	void onMoveWest();
	void onMoveSouth();
	void onMoveNorth();

	void onMoveEast10();
	void onMoveWest10();
	void onMoveSouth10();
	void onMoveNorth10();

	void onMoveUp();
	void onMoveDown();
	void onMoveUp10();
	void onMoveDown10();

private:
	void createWidgets();
	void doLayout();

	void setFloatingSize();

private:
	/** X/Y  Movement **/
	QPushButton* mpMoveEast = nullptr;
	QPushButton* mpMoveWest = nullptr;
	QPushButton* mpMoveSouth = nullptr;
	QPushButton* mpMoveNorth = nullptr;

	QPushButton* mpMoveEast10 = nullptr;
	QPushButton* mpMoveWest10 = nullptr;
	QPushButton* mpMoveSouth10 = nullptr;
	QPushButton* mpMoveNorth10 = nullptr;

	QLineEdit* mpHorizontalStepSize = nullptr;

	QPushButton* mpMoveUp = nullptr;
	QPushButton* mpMoveDown = nullptr;
	QPushButton* mpMoveUp10 = nullptr;
	QPushButton* mpMoveDown10 = nullptr;

	QLineEdit* mpVerticalStepSize = nullptr;

private:
	double mX_mm = 0;
	double mY_mm = 0;
	double mZ_mm = 0;

private:
	double mMinX_mm = 0;
	double mMaxX_mm = 0;
	double mMinY_mm = 0;
	double mMaxY_mm = 0;
	double mMinHeight_mm = 0;
	double mMaxHeight_mm = 0;
};