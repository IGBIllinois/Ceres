
#pragma once

#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
QT_END_NAMESPACE

class cSensorPropertyPage : public QWidget
{
	Q_OBJECT

public:
	cSensorPropertyPage(QWidget* parent = nullptr);
	virtual ~cSensorPropertyPage();

public:
	QAction* showAction() const;

	void setTitle(const QString& text);

public slots:
	void showPage();

private:
	QAction* mpShowAction;
	QDialogButtonBox* mpButtons;
};