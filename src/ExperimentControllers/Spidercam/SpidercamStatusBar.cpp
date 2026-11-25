
#include "SpidercamStatusBar.hpp"

#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>


cSpidercamStatusBar::cSpidercamStatusBar(QWidget *parent) : QStatusBar(parent)
{
	mpTitle = new QLabel(this);
	mpTitle->setMinimumWidth(200);

	addWidget(mpTitle);

	mpTitle->setText("Hello");
}

cSpidercamStatusBar::~cSpidercamStatusBar()
{
}

