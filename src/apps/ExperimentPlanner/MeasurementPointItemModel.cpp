
#pragma once

#include "MeasurementPointItemModel.hpp"

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

QPointItemModel::QPointItemModel(QObject* parent)
	: QAbstractItemModel(parent)
{}

QPointItemModel::QPointItemModel(int rows, QObject* parent)
	: QAbstractItemModel(parent)
{
//	setRow
}

QPointItemModel::~QPointItemModel()
{}

void QPointItemModel::appendRow(const std::string& id, int x_mm, int y_mm, ePointRepresentsScan type)
{}

int	QPointItemModel::rowCount(const QModelIndex& parent) const
{
	return 0;
}

QModelIndex	QPointItemModel::index(int row, int column, const QModelIndex& parent) const
{
	return QModelIndex();
}

QModelIndex	QPointItemModel::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

QVariant QPointItemModel::data(const QModelIndex& index, int role) const
{
	return QVariant();
}

void QPointItemModel::clear()
{}
