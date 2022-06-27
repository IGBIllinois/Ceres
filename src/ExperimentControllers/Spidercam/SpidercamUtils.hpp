
#pragma once

#include <spidercam/spidercam_types.hpp>

namespace spidercam
{

	template<typename T, T threshold>
	struct window_compare
	{
		static bool lower_threshold(const T& current, const T& previous) { return current < (previous - threshold); };
		static bool upper_threshold(const T& current, const T& previous) { return current > (previous + threshold); };
		static bool within_threshold(const T& current, const T& previous) { return ((previous - threshold) <= current) && (current <= (previous + threshold)); };
		static bool outside_threshold(const T& current, const T& previous) { return (current < (previous - threshold)) || (current > (previous + threshold)); };
	};

	bool hasPositionChanged(const sPosition& pos1, const sPosition& pos2, double tolerance_mm);
	bool hasPosSpeedChanged(const sPosition& pos1, const sPosition& pos2, double tolerance_mm);

}	// End of namespace: spidercam