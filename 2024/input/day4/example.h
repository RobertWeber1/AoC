#pragma once
#include <string_view>

using namespace std::literals;

namespace day4 {

struct Example
{
	static constexpr std::string_view data =
R"(................
................
................
...MMMSXXMASM...
...MSAMXMSMSA...
...AMXSXMAAMM...
...MSAMASMSMX...
...XMASAMXAMM...
...XXAMMXXAMA...
...SMSMSASXSS...
...SAXAMASAAA...
...MAMMMXMMMM...
...MXMXAXMASX...
................
................
................
)"sv;

};

} //namespace day4
