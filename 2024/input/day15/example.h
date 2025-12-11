#pragma once
#include <string_view>

using namespace std::literals;

namespace day15 {

struct Example
{
	static constexpr std::string_view map[] = {
		"########",
		"#..O.O.#",
		"##@.O..#",
		"#...O..#",
		"#.#.O..#",
		"#...O..#",
		"#......#",
		"########",
	};

	static constexpr std::string_view commands = "<^^>>>vv<v>>v<<";
};

} //namespace day15
