#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include "read_file.h"
#include "utils.h"

uint64_t count_zeros_part_one(std::string const& name, uint64_t dial = 50)
{
	MemoryMappedFile file(name);
	uint32_t result = 0;

	for_each_line(
		file.get(),
		[&dial, &result](std::string_view line){
			auto tmp = dial;

			auto number = string_view_to_int(std::string_view(&line[1], line.size()));

			if(line[0] == 'R')
			{
				dial = (dial + number) % 100;
			}
			else
			{
				number = number % 100;

				if(number <= dial)
				{
					dial -= number;
				}
				else
				{
					dial = 100 - (number - dial);
				}
			}


			if(dial == 0)
			{
				result++;
			}
		});

	return result;
}


uint64_t count_zeros_part_two(std::string const& name, uint64_t dial = 50)
{
	MemoryMappedFile file(name);
	uint32_t result = 0;

	for_each_line(
		file.get(),
		[&dial, &result](std::string_view line){
			auto tmp = dial;

			auto number = string_view_to_int(std::string_view(&line[1], line.size()));

			if(line[0] == 'R')
			{
				auto const sum = dial + number;
				result += sum /100;
				dial = sum % 100;
				if(dial == 0)
				{
					result--;
				}
			}
			else
			{
				result += number / 100;
				number = number % 100;

				if(number <= dial)
				{
					dial -= number;
				}
				else
				{
					bool already_zero = (dial == 0);
					dial = 100 - (number - dial);
					if(dial != 0 and not already_zero)
					{
						result++;
					}
				}
			}


			if(dial == 0)
			{
				result++;
			}
		});

	return result;
}

TEST_CASE("day 1-1")
{
	REQUIRE(count_zeros_part_one("./input/day1/example.txt") == 3);
	REQUIRE(count_zeros_part_one("./input/day1/input.txt") == 1168);
}

TEST_CASE("day 1-2")
{
	REQUIRE(count_zeros_part_two("./input/day1/example.txt") == 6);
	REQUIRE(count_zeros_part_two("./input/day1/input.txt") == 7199);
}
