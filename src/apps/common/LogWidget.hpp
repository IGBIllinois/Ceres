
#pragma once

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class QStackedLayout;
QT_END_NAMESPACE

class cLogWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    cLogWidget(QWidget* parent = nullptr);
    ~cLogWidget();

private:
};

