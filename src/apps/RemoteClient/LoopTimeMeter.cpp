
#include "LooptimeMeter.hpp"

#include <QLineEdit>


cLoopTimeMeter::cLoopTimeMeter(QWidget* parent)
:
    QStatusBar(parent), mpAvgLoopTime_ms(nullptr)
{
	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance(" XXX.XXX ");

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
{}


///////////////////////////////////////////////////////////////////////////////
// public slots:
///////////////////////////////////////////////////////////////////////////////

void cLoopTimeMeter::loopTimeUpdated(float avr_loop_time_ms)
{
	QString str = QString::number(avr_loop_time_ms, 'f', 3);
	mpAvgLoopTime_ms->setText(str);
}
