#pragma once
#include <string_view>

using namespace std::literals;

namespace day8 {

struct Example
{
	static constexpr std::string_view data[] = {
		R"(............)"sv,
		R"(........0...)"sv,
		R"(.....0......)"sv,
		R"(.......0....)"sv,
		R"(....0.......)"sv,
		R"(......A.....)"sv,
		R"(............)"sv,
		R"(............)"sv,
		R"(........A...)"sv,
		R"(.........A..)"sv,
		R"(............)"sv,
		R"(............)"sv};
};

} //namespace day7
