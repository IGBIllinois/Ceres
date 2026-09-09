
#pragma once

#include "ExperimentState.hpp"

#include <QObject>
#include <QString>

#include <nlohmann/json.hpp>


class cAboveGroundHeightDlg;

/**
 * An experiment state to ask for the parameters needed to compute 
 * aerial droop to stay at a constants height above the ground
 */
class cExperimentState_AGH : public cExperimentState
{
	Q_OBJECT

public:
	cExperimentState_AGH();
	~cExperimentState_AGH();

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

signals:
	void attentionAlert();
	void showDlg();

private:
	cAboveGroundHeightDlg* mpDlg;
};


class cAboveCanopyHeightDlg;

/**
 * An experiment state to ask for the parameters needed to compute
 * aerial droop to stay at a constants height above the plant canopy
 */

class cExperimentState_ACH : public cExperimentState
{
	Q_OBJECT

public:
	cExperimentState_ACH();
	~cExperimentState_ACH();

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

signals:
	void attentionAlert();
	void showDlg();

private:
	cAboveCanopyHeightDlg* mpDlg;
};



