
#include "QLogWidget.hpp"

#include <QApplication>
#include <QScrollBar>


QLogWidget::QLogWidget(QWidget* parent)
:
	QPlainTextEdit(parent)
{
	setReadOnly(true);
	setLineWrapMode(NoWrap);
}

QLogWidget::~QLogWidget()
{}

void QLogWidget::appendMessage(const QString& text)
{
	QString message = text;
	message += "\n";
	appendPlainText(message);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
