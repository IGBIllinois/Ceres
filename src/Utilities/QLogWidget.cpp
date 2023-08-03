
#include "QLogWidget.hpp"

#include <QApplication>
#include <QScrollBar>
#include <QFont>


QLogWidget::QLogWidget(QWidget* parent)
:
	QPlainTextEdit(parent)
{
	setReadOnly(true);
	setLineWrapMode(NoWrap);

	QFont font("Courier");
	setFont(font);
}

QLogWidget::~QLogWidget()
{}

void QLogWidget::logStatusMessage(const QString& device, const QString& msg)
{
	QString text = "STATUS:  ";
	text += device + ", ";
	text.resize(30, ' ');
	text += msg;
	appendPlainText(text);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QLogWidget::logInfoMessage(const QString& device, const QString& msg)
{
	QString text = "INFO:    ";
	text += device + ", ";
	text.resize(30, ' ');
	text += msg;
	appendPlainText(text);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QLogWidget::logWarningMessage(const QString& device, const QString& msg)
{
	QString text = "WARNING: ";
	text += device + ", ";
	text.resize(30, ' ');
	text += msg;
	appendPlainText(text);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QLogWidget::logErrorMessage(const QString& device, const QString& msg)
{
	QString text = "ERROR:   ";
	text += device + ", ";
	text.resize(30, ' ');
	text += msg;
	appendPlainText(text);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QLogWidget::logMessage(const QString& device, const QString& msg)
{
	QString text = "UNKNOWN: ";
	text += device + ", ";
	text.resize(30, ' ');
	text += msg;
	appendPlainText(text);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

