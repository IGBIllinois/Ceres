#pragma once


#include <vector>
#include <string>

namespace nRFM
{
	bool load_ground_data(const std::string& fileName);
	bool load_aerial_data(const std::string& fileName);

	int reference_height_mm();

	int ground_height_mm(int x_mm, int y_mm);

	bool has_reference_height_mm();

	int set_reference_height_mm(int geometric_height_mm, int x_mm, int y_mm, int z_mm);
	void set_reference_height_mm(int reference_height_mm);

	int compute_reference_height_mm(int geometric_height_mm, int x_mm, int y_mm, int z_mm);

	int dolly_offset_mm(int x_mm, int y_mm);
	int dolly_offset_mm(int x_mm, int y_mm, int ref_height_mm);

	int compute_dolly_height_mm(int height_mm, int x_mm, int y_mm, int ref_height_mm);
};
