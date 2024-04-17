
#pragma once

#include <QtWidgets>
#include <QDialog>

#include <filesystem>
#include <list>
#include <string>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

// Forward Declarations
class cExerimentStep;


class cExperimentFile
{
public:
	typedef std::list<cExerimentStep*> Experiment_t;

	typedef Experiment_t::iterator			iterator;
	typedef Experiment_t::const_iterator	const_iterator;

public:
	cExperimentFile();
	~cExperimentFile();

	const std::string& getFileName() const;

	bool isDirty() const;

	void clear();

	void open(const std::string& file_name);

	void save();

	void save_as(const std::string& file_name);

	bool empty() const;
	std::size_t size() const;

//	bool contains(const std::string& name);

	const cExerimentStep& front() const;
	cExerimentStep& front();

	iterator		begin();
	iterator		end();

	const_iterator	begin() const;
	const_iterator	end() const;

	void insertBefore(int index, cExerimentStep* step);
	void insertAfter(int index, cExerimentStep* step);

//	const_iterator	find(const std::string& name) const;
//	iterator		find(const std::string& name);

//	cPlotConfigScan& add(const std::string& name);
//	void remove(const std::string& name);

	const cExerimentStep& operator[](int index) const;
	cExerimentStep& operator[](int index);


private:
	std::filesystem::path	mExperimentPath;
	std::string	mFileName;

	std::list<cExerimentStep*> mSteps;
};


