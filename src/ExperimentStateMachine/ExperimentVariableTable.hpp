
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <any>
#include <map>


class cExperimentVariableTable
{
public:
	cExperimentVariableTable();
	~cExperimentVariableTable();

	void clear();

	void add(std::string_view name, bool value);

	void add(std::string_view name, int16_t value);
	void add(std::string_view name, uint16_t value);

	void add(std::string_view name, int32_t value);
	void add(std::string_view name, uint32_t value);

	void add(std::string_view name, int64_t value);
	void add(std::string_view name, uint64_t value);

	void add(std::string_view name, float value);
	void add(std::string_view name, double value);

	bool set(std::string_view name, bool value);

	bool set(std::string_view name, int16_t value);
	bool set(std::string_view name, uint16_t value);

	bool set(std::string_view name, int32_t value);
	bool set(std::string_view name, uint32_t value);

	bool set(std::string_view name, int64_t value);
	bool set(std::string_view name, uint64_t value);

	bool set(std::string_view name, float value);
	bool set(std::string_view name, double value);

	bool contains(std::string_view name) const;

	bool get(std::string_view name, bool& value) const;

	bool get(std::string_view name, int16_t& value) const;
	bool get(std::string_view name, uint16_t& value) const;

	bool get(std::string_view name, int32_t& value) const;
	bool get(std::string_view name, uint32_t& value) const;

	bool get(std::string_view name, int64_t& value) const;
	bool get(std::string_view name, uint64_t& value) const;

	bool get(std::string_view name, float& value) const;
	bool get(std::string_view name, double& value) const;

private:
	std::map<std::string, std::any> mNamedVariables;
};


