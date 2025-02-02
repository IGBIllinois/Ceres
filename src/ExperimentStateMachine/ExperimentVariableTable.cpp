
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



void cExperimentVariableTable::clear()
{
	mNamedVariables.clear();
}


void cExperimentVariableTable::add(const std::string& name, bool value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, int16_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, uint16_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, int32_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, uint32_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, int64_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, uint64_t value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, float value)
{
	mNamedVariables[name] = std::any(value);
}

void cExperimentVariableTable::add(const std::string& name, double value)
{
	mNamedVariables[name] = std::any(value);
}



bool cExperimentVariableTable::set(const std::string& name, bool value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, int16_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, uint16_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, int32_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, uint32_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, int64_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, uint64_t value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, float value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}

bool cExperimentVariableTable::set(const std::string& name, double value)
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return set_value(value, it->second);
}



bool cExperimentVariableTable::contains(const std::string& name) const
{
	return mNamedVariables.contains(name);
}



bool cExperimentVariableTable::get(const std::string& name, bool& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, int16_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, uint16_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, int32_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, uint32_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, int64_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, uint64_t& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, float& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}

bool cExperimentVariableTable::get(const std::string& name, double& value) const
{
	auto it = mNamedVariables.find(name);
	if (it == mNamedVariables.end())
		return false;

	return to_value(it->second, value);
}
