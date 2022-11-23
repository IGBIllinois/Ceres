

#include "OusterPropertyPage_Local.hpp"
#include "OusterModel.hpp"

#include <ouster/ouster_utils.h>

#include <QLineEdit>
#include <QComboBox>


cOusterPropertyPage_Local::cOusterPropertyPage_Local(cOusterModel* model, QWidget* parent)
	: cOusterPropertyPage(parent), mpModel(model)
{
	assert(mpModel);
}

/*
void cOusterPropertyPage_Local::onAzimuthWindow(double min_deg, double max_deg)
{
	mpMinAzimuthAngle_deg->setText(QString::number(min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(max_deg));

	mDefaultMinAzimuthAngle_deg = min_deg;
	mDefaultMaxAzimuthAngle_deg = max_deg;

	setEnabled(true);
	update();
}

void cOusterPropertyPage_Local::onLidarMode(const std::string& mode)
{
	QString qMode = QString::fromStdString(mode);

	auto n = mpLidarModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpLidarModes->itemText(i);
		if (0 == data.compare(qMode))
		{
			mDefaultLidarMode = qMode;
			mpLidarModes->setCurrentIndex(i);
			setEnabled(true);
			update();
			break;
		}
	}
}
*/

void cOusterPropertyPage_Local::doLayout()
{
	setTitle(QString::fromStdString(mpModel->name()));
	cOusterPropertyPage::doLayout();
}

void cOusterPropertyPage_Local::azimuthWindowChanged()
{
	auto window = mpModel->getAzimuthWindow();

	mpMinAzimuthAngle_deg->setText(QString::number(window.min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(window.max_deg));

	mDefaultMinAzimuthAngle_deg = window.min_deg;
	mDefaultMaxAzimuthAngle_deg = window.max_deg;

	setEnabled(true);
	update();
}

void cOusterPropertyPage_Local::dataFormatChanged()
{
	auto mode = mpModel->getLidarMode();

	QString qMode = QString::fromStdString(to_string(mode));

	auto n = mpLidarModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpLidarModes->itemText(i);
		if (0 == data.compare(qMode))
		{
			mDefaultLidarMode = qMode;
			mpLidarModes->setCurrentIndex(i);
			break;
		}
	}

	setEnabled(true);
	update();
}

void cOusterPropertyPage_Local::showPage()
{
	auto window = mpModel->getAzimuthWindow();
	auto mode = mpModel->getLidarMode();

	mpMinAzimuthAngle_deg->setText(QString::number(window.min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(window.max_deg));

	mDefaultMinAzimuthAngle_deg = window.min_deg;
	mDefaultMaxAzimuthAngle_deg = window.max_deg;

	QString qMode = QString::fromStdString(to_string(mode));

	auto n = mpLidarModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpLidarModes->itemText(i);
		if (0 == data.compare(qMode))
		{
			mDefaultLidarMode = qMode;
			mpLidarModes->setCurrentIndex(i);
			break;
		}
	}

	setEnabled(true);
	update();

	cOusterPropertyPage::showPage();
}

void cOusterPropertyPage_Local::doOK()
{
	doApply();
	cOusterPropertyPage::doOK();
}

void cOusterPropertyPage_Local::doCancel()
{
	cOusterPropertyPage::doCancel();
}

void cOusterPropertyPage_Local::doApply()
{
	bool enable = true;
	auto min_deg = mpMinAzimuthAngle_deg->text().toDouble();
	auto max_deg = mpMaxAzimuthAngle_deg->text().toDouble();

	if ((mDefaultMinAzimuthAngle_deg != min_deg) ||
		(mDefaultMaxAzimuthAngle_deg != max_deg))
	{
		emit requestNewAzimuthWindow(min_deg, max_deg);
		enable = false;
	}

	auto mode = mpLidarModes->currentText();
	if (mode.compare(mDefaultLidarMode) != 0)
	{
		emit requestNewLidarMode(mode);
		enable = false;
	}

	setEnabled(enable);
}


