#pragma once
#include <string_view>

using namespace std::literals;

namespace day2 {

struct Example
{
	static constexpr std::string_view data =
R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
)"sv;

};

} //namespace day2
