#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <fmt/ostream.h>
#include <set>

#include <day9/example.h>
#include <day9/input.h>

int calc_checksum(std::string_view data)
{
	int result = 0;

	return result;
}


TEST_CASE("day 9")
{
	SECTION("part 1")
	{
		REQUIRE(calc_checksum(day9::Example::data) == 1928);
	}

	SECTION("part 2")
	{
	}
}
