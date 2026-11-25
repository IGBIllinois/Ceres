
#include "ExperimentConfigFile.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>


namespace
{
	int to_pass_number(const std::string& name)
	{
		std::string num_str;

		for (auto it = name.rbegin(); it != name.rend(); ++it)
		{
			if (std::isdigit(*it) == 0)
				break;

			num_str += *it;
		}

		std::reverse(num_str.begin(), num_str.end());

		if (num_str.empty())
			return -1;

		return std::stoi(num_str);
	}
}

cExperimentConfigFile::cExperimentConfigFile()
{}

cExperimentConfigFile::~cExperimentConfigFile()
{}

const std::string& cExperimentConfigFile::getFileName() const
{
	return mFileName;
}

const std::string& cExperimentConfigFile::getTemporaryFileName() const
{
	return mTmpFileName;
}

bool cExperimentConfigFile::isDirty() const
{
	if (mMetaInfo.isDirty())
		return true;

	return mIsDirty;
}

void cExperimentConfigFile::clear()
{
	mMetaInfo.clear();

	mIsDirty = false;
	mFileName.clear();
	mTmpFileName = "~newfile.plotsplit";
}

bool cExperimentConfigFile::open(const std::string& file_name)
{
//	using namespace nStringUtils;

	if (file_name.empty()) return false;

//	if (is_temp_filename(file_name))
//		return open_temporary_file(file_name);

	std::ifstream in;
	in.open(file_name);
	if (!in.is_open())
		return false;

	nlohmann::json configDoc;
	try
	{
		in >> configDoc;
	}
	catch (const nlohmann::json::parse_error& e)
	{
		auto msg = e.what();
		throw std::runtime_error(msg);
	}
	catch (const std::exception& e)
	{
		return false;
	}

	mFileName = file_name;
//	mTmpFileName = nStringUtils::make_temp_filename(mFileName);

	mIsDirty = false;

	return true;
}

void cExperimentConfigFile::save()
{
	if (mFileName.empty()) return;

	nlohmann::json configDoc;

	buildDocument(configDoc);

	std::ofstream out;
	out.open(mFileName, std::ios::trunc);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;

	if (out.good())
	{
		// Remove the temporary file
		std::filesystem::path tmp = mTmpFileName;
		if (std::filesystem::exists(tmp))
		{
			std::filesystem::remove(tmp);
		}
	}

	clearDirtyFlag();
}

void cExperimentConfigFile::save_as(const std::string& file_name)
{
	nlohmann::json configDoc;

	buildDocument(configDoc);

	std::ofstream out;
	out.open(file_name);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;

	if (out.good())
	{
		// Remove the temporary file
		std::filesystem::path tmp = mTmpFileName;
		if (std::filesystem::exists(tmp))
		{
			std::filesystem::remove(tmp);
		}
	}

	mFileName = file_name;
//	mTmpFileName = nStringUtils::make_temp_filename(mFileName);

	clearDirtyFlag();
}

bool cExperimentConfigFile::open_temporary_file(const std::string& file_name)
{
	if (file_name.empty()) return false;
//	if (!nStringUtils::is_temp_filename(file_name)) return false;

	std::ifstream in;
	in.open(file_name);
	if (!in.is_open())
		return false;

	nlohmann::json configDoc;
	try
	{
		in >> configDoc;
	}
	catch (const nlohmann::json::parse_error& e)
	{
		auto msg = e.what();
		return false;
	}
	catch (const std::exception& e)
	{
		return false;
	}

	mTmpFileName = file_name;

//	mFileName = nStringUtils::convert_temp_filename(mTmpFileName);

	if (mFileName.find("newfile") != std::string::npos)
	{
		mFileName.clear();
	}

	mIsDirty = true;

	return true;
}

void cExperimentConfigFile::save_temporary_file()
{
	nlohmann::json configDoc;

	buildDocument(configDoc);

	std::ofstream out;
	out.open(mTmpFileName, std::ios::trunc);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;
}


void cExperimentConfigFile::buildDocument(nlohmann::json& configDoc)
{
	mMetaInfo.save(configDoc);
}

bool cExperimentConfigFile::empty() const
{
	return true; // mScans.empty();
}

std::size_t cExperimentConfigFile::size() const
{
	return 0; // mScans.size();
}

const cExperimentInfo& cExperimentConfigFile::getMetaData() const
{
	return mMetaInfo;
}

cExperimentInfo& cExperimentConfigFile::getMetaData()
{
	return mMetaInfo;
}

void cExperimentConfigFile::setMetaData(const cExperimentInfo& meta_info)
{
	mIsDirty |= mMetaInfo != meta_info;
	mMetaInfo = meta_info;
}

void cExperimentConfigFile::clearDirtyFlag()
{
	mIsDirty = false;

}
