#pragma once
#include <string_view>

using namespace std::literals;

namespace day3 {

struct Example
{
	static constexpr std::string_view data =
R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
)"sv;

};

} //namespace day3
