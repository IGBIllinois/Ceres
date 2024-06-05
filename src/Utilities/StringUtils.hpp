#pragma once

#include <string>
#include <vector>


namespace nStringUtils
{
	/**
	 * Case insensitive compare of two strings
	 */
	bool iequal(const std::string& lhs, const char* const rhs);

	/**
	 * Test is the string (sub_str) is in the string in.
	 */
	bool icontains(const std::string& in, const char* const sub_str);

	/**
	 * Extract a list of parameters, comma separated, contained
	 * between curly brackets {}
	 */
	std::vector<std::string> get_parameters(const std::string& str);

	/**
	 * Add a measurement timestamp to a filename
	 *
	 * The filename will become: filename_YYYYmmddHHMM.extension
	 */
	std::string addMeasurementTimestamp(std::string filename);

	/**
	 * Add a process timestamp to a filename
	 *
	 * The filename will become: filename.YYYYmmddHHMM.extension
	 */
	std::string addProcessedTimestamp(std::string filename);


	struct sFilenameAndExtension
	{
		std::string filename;
		std::string extension;
	};

	sFilenameAndExtension removeMeasurementTimestamp(const std::string& filename);

	sFilenameAndExtension removeProcessedTimestamp(const std::string& filename);

	/**
	 * Converts a potential filename to a safe file name
	 *
	 * 	Replaces characters: :, \\, /, <, >, |, ?, *, ", and space
	 *  with _
	 */
	std::string safeFilename(std::string filename);

	/**
	 * Compacts a filename to a maximum length
	 *
	 * The filename: d:\test\long_file_name.extension -> d:\...\long_file_name
	 */
	std::string compactFilename(const std::string& filename, std::size_t max_len);

	/**
	 * Tests to see in a string ends with a integer.  Optionally returns the integer;
	 *
	 * Test_1 will return true with the value set to 1
	 */
	bool endsWithInt(const std::string& str, int* pValue = nullptr);

	/**
	 * Replaces the integer at the end of a string with a new value.
	 *
	 * If the string does not end with an integer, the string is left unchanged
	 */
	void replaceIntAtEnd(std::string& str, int value);

} // End of Namespace nStringUtils