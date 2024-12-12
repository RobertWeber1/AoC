#pragma once
#include <string_view>

using namespace std::literals;

namespace day12 {

struct Example
{
	static constexpr std::string_view data1[] = {
		"AAAA",
		"BBCD",
		"BBCC",
		"EEEC",
	};

	static constexpr std::string_view data2[] = {
		"OOOOO",
		"OXOXO",
		"OOOOO",
		"OXOXO",
		"OOOOO",
	};

	static constexpr std::string_view data3[] = {
		"RRRRIICCFF",
		"RRRRIICCCF",
		"VVRRRCCFFF",
		"VVRCCCJFFF",
		"VVVVCJJCFE",
		"VVIVCCJJEE",
		"VVIIICJJEE",
		"MIIIIIJJEE",
		"MIIISIJEEE",
		"MMMISSJEEE",
	};
};

} //namespace day12
