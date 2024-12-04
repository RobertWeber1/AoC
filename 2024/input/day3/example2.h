#pragma once
#include <string_view>

using namespace std::literals;

namespace day3 {

struct Example2
{
	static constexpr std::string_view data =
R"(xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
)"sv;

};

} //namespace day3

