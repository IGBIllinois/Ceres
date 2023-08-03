
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
    void logStatusMessage(const QString& device, const QString& msg);
    void logInfoMessage(const QString& device, const QString& msg);
    void logWarningMessage(const QString& device, const QString& msg);
    void logErrorMessage(const QString& device, const QString& msg);

    void logMessage(const QString& device, const QString& msg);
};


