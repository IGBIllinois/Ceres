
#pragma once

#include <QObject>
#include <QString>

#include <nlohmann/json.hpp>
#include <memory>


class cExperimentVariableTable;


class cExperimentState : public QObject
{
	Q_OBJECT

public:
	enum class eRESULT 
	{
		WAITING,	// The state is not complete and the state machine should not advance
		DONE,		// The state is complete and the state machine should advance
		ABORT		// The state is requesting an abort of the experiment
	};

	cExperimentState(QObject* parent = nullptr);
	virtual ~cExperimentState();

	virtual QString getStatusStr() = 0;

	void attachVariableTable(std::weak_ptr<cExperimentVariableTable> vars);

	virtual bool configure(const nlohmann::json& stateDoc) = 0;
	virtual void cleanup() {};

	virtual bool needsDataFile();
	virtual bool recording() = 0;

	virtual bool initialize() = 0;
	virtual void run() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual eRESULT finished() = 0;

signals:
	void attentionAlert();

	void statusUpdate(QString msg);
	void errorUpdate(QString msg);

protected:
	std::weak_ptr<cExperimentVariableTable> mVariables;
};


