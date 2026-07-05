
#pragma once

#include "../../Utilities/Utilities.hpp"

#include <QWidget>

#include <vector>
#include <string>
#include <array>
#include <tuple>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QComboBox;
class QAbstractItemModel;
class QPointItemModel;
class QTableWidget;
class QTableView;
class QVBoxLayout;
QT_END_NAMESPACE


class cScanPointsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cScanPointsWidget(QWidget* parent = nullptr);
	~cScanPointsWidget();

private:
    void createControls_PointSelection();

    void createLayout_PointSelection();

private:
//    QTableWidget* mpStartPosition = nullptr;

    QLineEdit*  mpStartLabel = nullptr;
    QLineEdit*  mpStartX_mm = nullptr;
    QLineEdit*  mpStartY_mm = nullptr;
    QComboBox*  mpStartType = nullptr;

    QTableView* mpMeasurementPoints = nullptr;
    QPointItemModel* mpModel = nullptr;

//    QTableWidget* mpEndPosition = nullptr;

    QLineEdit* mpEndLabel = nullptr;
    QLineEdit* mpEndX_mm = nullptr;
    QLineEdit* mpEndY_mm = nullptr;
    QComboBox* mpEndType = nullptr;
};

