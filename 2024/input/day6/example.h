#pragma once
#include <string_view>

using namespace std::literals;

namespace day6 {

struct Example
{
	static constexpr std::string_view data[] = {
		"....#....."sv,
		".........#"sv,
		".........."sv,
		"..#......."sv,
		".......#.."sv,
		".........."sv,
		".#..^....."sv,
		"........#."sv,
		"#........."sv,
		"......#..."sv
	};
};

} //namespace day6
