#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>

#include <day3/example.h>
#include <day3/example2.h>
#include <day3/input.h>

constexpr int exec_mul(std::string_view data)
{
	int sum = 0;
	for (auto match :
		ctre::search_all<
			"mul\\("
				"(\\d{1,3}),"
				"(\\d{1,3})\\)">(data))
	{
		sum +=
			string_view_to_int(std::string_view(match.get<1>()))*
			string_view_to_int(std::string_view(match.get<2>()));
	}
	return sum;
}

constexpr int exec_do_mul(std::string_view data)
{
	int sum = 0;
	bool do_mul = true;
	for (auto match :
		ctre::search_all<
			"mul\\((\\d{1,3}),(\\d{1,3})\\)"
			"|don't\\(\\)"
			"|do\\(\\)">(data))
	{
		if(std::string_view(match.get<0>()) == "do()")
		{
			do_mul = true;
			continue;
		}

		if(std::string_view(match.get<0>()) == "don't()")
		{
			do_mul = false;
			continue;
		}
		if(do_mul)
		{
			sum +=
				string_view_to_int(std::string_view(match.get<1>()))*
				string_view_to_int(std::string_view(match.get<2>()));
			}
	}
	return sum;
}

TEST_CASE("day 3")
{
	SECTION("part 1")
	{
		AOC_REQUIRE(exec_mul(day3::Example::data) == 161);
		AOC_REQUIRE(exec_mul(day3::Input::data) == 185797128);
	}

	SECTION("part 2")
	{
		AOC_REQUIRE(exec_do_mul(day3::Example2::data) == 48);
		AOC_REQUIRE(exec_do_mul(day3::Input::data) == 89798695);
	}
}
