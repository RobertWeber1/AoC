#pragma once
#include <string_view>

using namespace std::literals;

namespace day9 {

struct Example
{
	static constexpr std::string_view data0 = R"(12345)"sv;
	static constexpr std::string_view data1 = R"(2333133121414131402)"sv;
};

} //namespace day9
