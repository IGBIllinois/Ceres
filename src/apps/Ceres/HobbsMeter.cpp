
#include "HobbsMeter.hpp"
#include "ExperimentTypes.hpp"

#include <QLineEdit>


cHobbsMeter::cHobbsMeter(QWidget* parent)
:
    QStatusBar(parent), mpExperimentTime_min(nullptr), mpElapseTime_hr(nullptr),
	mpTotalTime_hr(nullptr), mTimer(parent), mExperimentTimer(parent)
{
	QObject::connect(&mTimer, &QTimer::timeout, this, &cHobbsMeter::updateTime);
	mTimer.setInterval(60'000);

	QObject::connect(&mExperimentTimer, &QTimer::timeout, this, &cHobbsMeter::updateExpTime);
	mExperimentTimer.setInterval(3'000);

	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance(" XXX.X ");

	mpExperimentTime_min = new QLineEdit(this);
	mpExperimentTime_min->setReadOnly(true);
	mpExperimentTime_min->setFixedWidth(pixelsWide);
	mpExperimentTime_min->setAlignment(Qt::AlignCenter);
	mpExperimentTime_min->setToolTip(tr("Experiment Time (min)"));
	mpExperimentTime_min->setText("0.0");

	pixelsWide = fm.horizontalAdvance(" XXXXX.X ");

	mpElapseTime_hr = new QLineEdit(this);
	mpElapseTime_hr->setReadOnly(true);
	mpElapseTime_hr->setFixedWidth(pixelsWide);
	mpElapseTime_hr->setAlignment(Qt::AlignCenter);
	mpElapseTime_hr->setToolTip(tr("Elapse Time (hr)"));
	mpElapseTime_hr->setText("0.0");

	pixelsWide = fm.horizontalAdvance(" XXXXX.X ");
	mpTotalTime_hr = new QLineEdit(this);
	mpTotalTime_hr->setReadOnly(true);
	mpTotalTime_hr->setFixedWidth(pixelsWide);
	mpTotalTime_hr->setAlignment(Qt::AlignCenter);
	mpTotalTime_hr->setToolTip(tr("Total Time (hr)"));
	mpTotalTime_hr->setText("0.0");

	addPermanentWidget(mpExperimentTime_min);
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

void cHobbsMeter::onExperimentStateChange(experiment::eState state)
{
	using namespace experiment;

	if (state == eState::RUNNING)
	{
		if (!mRecordingExperimentTime)
		{
			mRecordingExperimentTime = true;
//			mExperimentTime_min = 0.0;
//			mpExperimentTime_min->setText("0.0");
//			mpExperimentTime_min->update();

			mExpStartTime = std::chrono::steady_clock::now();
			mExperimentTimer.start();
		}
	}
	else if ((state == eState::COMPLETED) || (state == eState::TERMINATED))
	{
		mRecordingExperimentTime = false;
		mExperimentTimer.stop();
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

void cHobbsMeter::updateExpTime()
{
	if (mRecordingExperimentTime)
	{
		auto endTime = std::chrono::steady_clock::now();
		auto diff_s = std::chrono::duration_cast<std::chrono::seconds>(endTime - mExpStartTime).count();

		if (diff_s >= 6)	// Wait every 6 seconds (0.1 of a minute)
		{
			float t = (diff_s / 60.0f); // Convert the time difference to minutes
			mExperimentTime_min += t;

			mExpStartTime = endTime;

			QString str = QString::number(mExperimentTime_min, 'f', 1);
			mpExperimentTime_min->setText(str);
		}
	}
	else
		mExperimentTimer.stop();
}

void cHobbsMeter::updateTimeData()
{
	QString str = QString::number(mElapseTime_hr, 'f', 1);
	mpElapseTime_hr->setText(str);

	str = QString::number(mTotalTime_hr, 'f', 1);
	mpTotalTime_hr->setText(str);
	mSettings.setValue("hobbs_meter/total_time_hr", mTotalTime_hr);
}
