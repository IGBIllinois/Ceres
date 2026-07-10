
#pragma once


#include <QAbstractItemModel>

#include <vector>
#include <string>


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

enum ePointRepresentsScan { START, END, CENTER, POINT };

struct sMeasurementPoint
{
    int x_mm = 0;
    int y_mm = 0;
    ePointRepresentsScan scan = START;
};


class QPointItemModel : public QAbstractItemModel
{
public:
	QPointItemModel(QObject* parent = nullptr);
	explicit QPointItemModel(int rows, QObject* parent = nullptr);
	virtual	~QPointItemModel();

	void appendRow(const std::string& id, int x_mm, int y_mm, ePointRepresentsScan type = ePointRepresentsScan::CENTER);

	int	rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int	columnCount(const QModelIndex& parent = QModelIndex()) const override {	return 4; };

	QModelIndex	index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex	parent(const QModelIndex& child) const override;
	QVariant	data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void clear();


};
