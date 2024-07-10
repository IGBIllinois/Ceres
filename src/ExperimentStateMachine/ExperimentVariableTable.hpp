
#pragma once

#include <cstdint>
#include <string>
#include <any>
#include <map>


class cExperimentVariableTable
{
public:
	cExperimentVariableTable() = default;

	void clear();

	void add(const std::string& name, bool value);

	void add(const std::string& name, int16_t value);
	void add(const std::string& name, uint16_t value);

	void add(const std::string& name, int32_t value);
	void add(const std::string& name, uint32_t value);

	void add(const std::string& name, int64_t value);
	void add(const std::string& name, uint64_t value);

	void add(const std::string& name, float value);
	void add(const std::string& name, double value);

	bool set(const std::string& name, bool value);

	bool set(const std::string& name, int16_t value);
	bool set(const std::string& name, uint16_t value);

	bool set(const std::string& name, int32_t value);
	bool set(const std::string& name, uint32_t value);

	bool set(const std::string& name, int64_t value);
	bool set(const std::string& name, uint64_t value);

	bool set(const std::string& name, float value);
	bool set(const std::string& name, double value);

	bool contains(const std::string& name) const;

	bool get(const std::string& name, bool& value) const;

	bool get(const std::string& name, int16_t& value) const;
	bool get(const std::string& name, uint16_t& value) const;

	bool get(const std::string& name, int32_t& value) const;
	bool get(const std::string& name, uint32_t& value) const;

	bool get(const std::string& name, int64_t& value) const;
	bool get(const std::string& name, uint64_t& value) const;

	bool get(const std::string& name, float& value) const;
	bool get(const std::string& name, double& value) const;

private:
	std::map<std::string, std::any> mNamedVariables;
};


