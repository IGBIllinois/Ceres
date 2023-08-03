
#pragma once

#include <QWidget>
#include <QPlainTextEdit>


/**
 * A basic widget for log messages
 */
class QLogWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    QLogWidget(QWidget* parent = nullptr);
	virtual ~QLogWidget();

public:
    void appendMessage(const QString& text);
};


