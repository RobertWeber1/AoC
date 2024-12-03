#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>

const MemoryMappedFile example(FileDescriptor("./input/day3/example.txt"));
const MemoryMappedFile example2(FileDescriptor("./input/day3/example2.txt"));
const MemoryMappedFile input(FileDescriptor("./input/day3/input.txt"));


int exec_mul(std::string_view && data)
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

int exec_do_mul(std::string_view && data)
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
		REQUIRE(exec_mul(example.get()) == 161);
		REQUIRE(exec_mul(input.get()) == 185797128);
	}

	SECTION("part 2")
	{
		REQUIRE(exec_do_mul(example2.get()) == 48);
		REQUIRE(exec_do_mul(input.get()) == 89798695);
	}
}
