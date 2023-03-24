
#include "HobbsMeter.hpp"

#include <QLineEdit>


cHobbsMeter::cHobbsMeter(QWidget* parent)
:
    QStatusBar(parent), mpElapseTime_hr(nullptr), mpTotalTime_hr(nullptr), mTimer(parent)
{
	QObject::connect(&mTimer, &QTimer::timeout, this, &cHobbsMeter::updateTime);
	mTimer.setInterval(60'000);

	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance(" XXXXX.X ");

	mpElapseTime_hr = new QLineEdit(this);
	mpElapseTime_hr->setReadOnly(true);
	mpElapseTime_hr->setFixedWidth(pixelsWide);
	mpElapseTime_hr->setAlignment(Qt::AlignCenter);
	mpElapseTime_hr->setToolTip(tr("Elapse Time (hr)"));
	mpElapseTime_hr->setText("Elapse");

	pixelsWide = fm.horizontalAdvance(" XXXXX.X ");
	mpTotalTime_hr = new QLineEdit(this);
	mpTotalTime_hr->setReadOnly(true);
	mpTotalTime_hr->setFixedWidth(pixelsWide);
	mpTotalTime_hr->setAlignment(Qt::AlignCenter);
	mpTotalTime_hr->setToolTip(tr("Total Time (hr)"));
	mpTotalTime_hr->setText("Total");

	addPermanentWidget(mpElapseTime_hr);
	addPermanentWidget(mpTotalTime_hr);

	setWindowTitle("Hobbs Meter");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	mTotalTime_hr = mSettings.value("hobbs_meter/total_time_hr", 0).toFloat();
	mStartTime = std::chrono::steady_clock::now();

	updateTimeData();
}

cHobbsMeter::~cHobbsMeter()
{
	mSettings.setValue("hobbs_meter/total_time_hr", mTotalTime_hr);
}


///////////////////////////////////////////////////////////////////////////////
// public slots:
///////////////////////////////////////////////////////////////////////////////

void cHobbsMeter::updateControllerConnection(bool connected)
{
	if (connected)
	{
		mStartTime = std::chrono::steady_clock::now();
		mTimer.start();
	}
	else
	{
		mTimer.stop();
	}
}

void cHobbsMeter::updateTime()
{
	auto endTime = std::chrono::steady_clock::now();
	auto diff_s = std::chrono::duration_cast<std::chrono::seconds>(endTime - mStartTime).count();

	if (diff_s >= 360)	// Wait every 6 minutes (0.1 of a hour)
	{
		float t = (diff_s / 3600.0f); // Convert the time difference to hours
		mElapseTime_hr += t;
		mTotalTime_hr += t;

		mStartTime = endTime;
		updateTimeData();
	}
}

void cHobbsMeter::updateTimeData()
{
	QString et = QString::number(mElapseTime_hr, 'f', 1);
	mpElapseTime_hr->setText(et);

	QString tt = QString::number(mTotalTime_hr, 'f', 1);
	mpTotalTime_hr->setText(tt);
	mSettings.setValue("hobbs_meter/total_time_hr", mTotalTime_hr);
}
