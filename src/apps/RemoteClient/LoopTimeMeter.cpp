
#include "LooptimeMeter.hpp"

#include <QLineEdit>
#include <QLabel >
#include <QPixmap>


cLoopTimeMeter::cLoopTimeMeter(QWidget* parent)
:
    QStatusBar(parent), mpAvgLoopTime_ms(nullptr)
{
	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance(" XXX.XXX ");
	int pixelsHeight = fm.boundingRect(" XXX.XXX ").height();

	QPixmap pixmap;
	if (pixmap.load(":/ripe.illinois.edu/heart_red.png"))
	{
		mpRedHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
	}
	else
		mpRedHeart = new QPixmap(":/ripe.illinois.edu/heart_red.png");

	if (pixmap.load(":/ripe.illinois.edu/heart_empty.png"))
	{
		mpEmptyHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
	}
	else
		mpEmptyHeart = new QPixmap(":/ripe.illinois.edu/heart_empty.png");

	if (pixmap.load(":/ripe.illinois.edu/heart_black.png"))
	{
		mpBlackHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
	}
	else
		mpBlackHeart = new QPixmap(":/ripe.illinois.edu/heart_black.png");

	mpHeartBeat = new QLabel(this);
	mpHeartBeat->setPixmap(*mpEmptyHeart);
	mShowRedHeart = true;

	addPermanentWidget(mpHeartBeat);


	mpAvgLoopTime_ms = new QLineEdit(this);
	mpAvgLoopTime_ms->setReadOnly(true);
	mpAvgLoopTime_ms->setFixedWidth(pixelsWide);
	mpAvgLoopTime_ms->setAlignment(Qt::AlignCenter);
	mpAvgLoopTime_ms->setToolTip(tr("Avg Loop Time (ms)"));
	mpAvgLoopTime_ms->setText("0.000");

	addPermanentWidget(mpAvgLoopTime_ms);
	setWindowTitle("Loop Time Meter");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

cLoopTimeMeter::~cLoopTimeMeter()
{
	delete mpRedHeart;   mpRedHeart = nullptr;
	delete mpEmptyHeart; mpEmptyHeart = nullptr;
	delete mpBlackHeart; mpBlackHeart = nullptr;
}


///////////////////////////////////////////////////////////////////////////////
// public slots:
///////////////////////////////////////////////////////////////////////////////

void cLoopTimeMeter::loopHeartbeatUpdated()
{
	if (mShowRedHeart)
	{
		mpHeartBeat->setPixmap(*mpRedHeart);
		mShowRedHeart = false;
	}
	else
	{
		mpHeartBeat->setPixmap(*mpEmptyHeart);
		mShowRedHeart = true;
	}
}

void cLoopTimeMeter::loopTimeUpdated(float avr_loop_time_ms)
{
	QString str = QString::number(avr_loop_time_ms, 'f', 3);
	mpAvgLoopTime_ms->setText(str);
}

void cLoopTimeMeter::loopTerminated()
{
	mpAvgLoopTime_ms->setText("-----");
	mpHeartBeat->setPixmap(*mpBlackHeart);
}
