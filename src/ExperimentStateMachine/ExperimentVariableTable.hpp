
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <any>
#include <map>


class cExperimentVariableTable
{
public:
	using key_type = std::string;
	using mapped_type = std::any;
	using value_type = std::pair<const std::string, std::any>;
	using pointer = typename std::map<std::string, std::any>::pointer;
	using const_pointer = typename std::map<std::string, std::any>::const_pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = typename std::map<std::string, std::any>::iterator;
	using const_iterator = typename std::map<std::string, std::any>::const_iterator;
	using reverse_iterator = typename std::map<std::string, std::any>::reverse_iterator;
	using const_reverse_iterator = typename std::map<std::string, std::any>::const_reverse_iterator;

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

	// Iterators
	iterator begin() { return mNamedVariables.begin(); };
	const_iterator begin() const { return mNamedVariables.begin(); };
	const_iterator cbegin() const noexcept { return mNamedVariables.cbegin(); };

	iterator end() { return mNamedVariables.end(); };
	const_iterator end() const { return mNamedVariables.end(); };
	const_iterator cend() const noexcept { return mNamedVariables.cend(); };

	reverse_iterator rbegin() { return mNamedVariables.rbegin(); };
	const_reverse_iterator rbegin() const { return mNamedVariables.rbegin(); };
	const_reverse_iterator crbegin() const noexcept { return mNamedVariables.crbegin(); };

	reverse_iterator rend() { return mNamedVariables.rend(); };
	const_reverse_iterator rend() const { return mNamedVariables.rend(); };
	const_reverse_iterator crend() const noexcept { return mNamedVariables.crend(); };

	bool insert(const value_type& value);

private:
	std::map<std::string, std::any> mNamedVariables;
};


