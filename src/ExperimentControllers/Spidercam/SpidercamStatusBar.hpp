
#pragma once

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


class cSpidercamStatusBar : public QStatusBar
{
public:
	cSpidercamStatusBar(QWidget *parent = nullptr);
	virtual ~cSpidercamStatusBar();

private:
	QLabel* mpTitle = nullptr;
};

