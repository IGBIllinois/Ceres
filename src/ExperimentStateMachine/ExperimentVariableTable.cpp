
#include "ExperimentVariableTable.hpp"

namespace
{
	template<typename T>
	bool to_value(const std::any& a, T& value)
	{
		if (a.type() == typeid(T))
		{
			value = std::any_cast<T>(a);
			return true;
		}

		if (a.type() == typeid(int16_t))
		{
			value = static_cast<T>(std::any_cast<int16_t>(a));
			return true;
		}

		if (a.type() == typeid(uint16_t))
		{
			value = static_cast<T>(std::any_cast<uint16_t>(a));
			return true;
		}

		if (a.type() == typeid(int32_t))
		{
			value = static_cast<T>(std::any_cast<int32_t>(a));
			return true;
		}

		if (a.type() == typeid(uint32_t))
		{
			value = static_cast<T>(std::any_cast<uint32_t>(a));
			return true;
		}

		if (a.type() == typeid(int64_t))
		{
			value = static_cast<T>(std::any_cast<int64_t>(a));
			return true;
		}

		if (a.type() == typeid(uint64_t))
		{
			value = static_cast<T>(std::any_cast<uint64_t>(a));
			return true;
		}

		if (a.type() == typeid(float))
		{
			value = static_cast<T>(std::any_cast<float>(a));
			return true;
		}

		if (a.type() == typeid(double))
		{
			value = static_cast<T>(std::any_cast<double>(a));
			return true;
		}

		return false;
	}

	template<>
	bool to_value<bool>(const std::any& a, bool& value)
	{
		if (a.type() == typeid(bool))
		{
			value = std::any_cast<bool>(a);
			return true;
		}

		return false;
	}

	template<typename T>
	bool set_value(const T value, std::any& a)
	{
		if (a.type() == typeid(T))
		{
			a = value;
			return true;
		}

		if (a.type() == typeid(int16_t))
		{
			a = static_cast<int16_t>(value);
			return true;
		}

		if (a.type() == typeid(uint16_t))
		{
			a = static_cast<uint16_t>(value);
			return true;
		}

		if (a.type() == typeid(int32_t))
		{
			a = static_cast<int32_t>(value);
			return true;
		}

		if (a.type() == typeid(uint32_t))
		{
			a = static_cast<uint32_t>(value);
			return true;
		}

		if (a.type() == typeid(int64_t))
		{
			a = static_cast<int64_t>(value);
			return true;
		}

		if (a.type() == typeid(uint64_t))
		{
			a = static_cast<uint64_t>(value);
			return true;
		}

		if (a.type() == typeid(float))
		{
			a = static_cast<float>(value);
			return true;
		}

		if (a.type() == typeid(double))
		{
			a = static_cast<double>(value);
			return true;
		}

		return false;
	}

	template<>
	bool set_value<bool>(const bool value, std::any& a)
	{
		if (a.type() == typeid(bool))
		{
			a = value;
			return true;
		}

		return false;
	}

}	// End of namespace

cExperimentVariableTable::cExperimentVariableTable()
{}

cExperimentVariableTable::~cExperimentVariableTable()
{
	mNamedVariables.clear();
}

void cExperimentVariableTable::clear()
{
	mNamedVariables.clear();
}


void cExperimentVariableTable::add(std::string_view name, bool value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, int16_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, uint16_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, int32_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, uint32_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, int64_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, uint64_t value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, float value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}

void cExperimentVariableTable::add(std::string_view name, double value)
{
	mNamedVariables[std::string(name)] = std::any(value);
}



bool cExperimentVariableTable::set(std::string_view name, bool value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, int16_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, uint16_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, int32_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, uint32_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, int64_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, uint64_t value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, float value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(std::string_view name, double value)
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}



bool cExperimentVariableTable::contains(std::string_view name) const
{
	return mNamedVariables.contains(std::string(name));
}



bool cExperimentVariableTable::get(std::string_view name, bool& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, int16_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, uint16_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, int32_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, uint32_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, int64_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, uint64_t& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, float& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(std::string_view name, double& value) const
{
	auto it = mNamedVariables.find(std::string(name));
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::insert(const value_type& value)
{
	return mNamedVariables.insert(value).second;
}
