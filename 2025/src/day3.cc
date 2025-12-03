#include <catch2/catch_test_macros.hpp>
#include <charconv>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "read_file.h"
#include "utils.h"


std::vector<uint8_t> make_bank(std::string_view line)
{
	std::vector<uint8_t> result;
	result.reserve(line.size());
	for(char c : line)
	{
		result.push_back(c-0x30);
	}
	return result;
}

int biggest_number_index(std::vector<uint8_t> const& bank, int begin, int end)
{
	int biggest_index = begin;
	for(int i = begin; i<end; ++i)
	{
		if(bank[i] > bank[biggest_index])
		{
			biggest_index = i;
		}

		if(bank[biggest_index] == 9)
		{
			break;
		}
	}

	return biggest_index;
}


uint64_t get_max_joltage_part_1(std::vector<uint8_t> const& bank)
{
	auto const i1 = biggest_number_index(bank,     0, bank.size() - 1);
	auto const i0 = biggest_number_index(bank,  i1+1, bank.size() - 0);

	return
		uint64_t(bank[i1])  * 10 +
		uint64_t(bank[i0])  * 1;
}

uint64_t sum_joltage_part_1(std::string const& name)
{
	uint64_t result = 0;

	MemoryMappedFile file(name);

	for_each_line(
		file.get(),
		[&result](std::string_view line){
			auto const jolts = get_max_joltage_part_1(make_bank(line));
			result += jolts;
			// fmt::println("line: {} -> {}", line, jolts);
		});

	return result;
}


//------------------------------------------------------------------------------



uint64_t get_max_joltage_part_2(std::vector<uint8_t> const& bank)
{
	auto const i11 = biggest_number_index(bank,     0, bank.size() - 11);
	auto const i10 = biggest_number_index(bank, i11+1, bank.size() - 10);
	auto const i9  = biggest_number_index(bank, i10+1, bank.size() - 9);
	auto const i8  = biggest_number_index(bank,  i9+1, bank.size() - 8);
	auto const i7  = biggest_number_index(bank,  i8+1, bank.size() - 7);
	auto const i6  = biggest_number_index(bank,  i7+1, bank.size() - 6);
	auto const i5  = biggest_number_index(bank,  i6+1, bank.size() - 5);
	auto const i4  = biggest_number_index(bank,  i5+1, bank.size() - 4);
	auto const i3  = biggest_number_index(bank,  i4+1, bank.size() - 3);
	auto const i2  = biggest_number_index(bank,  i3+1, bank.size() - 2);
	auto const i1  = biggest_number_index(bank,  i2+1, bank.size() - 1);
	auto const i0  = biggest_number_index(bank,  i1+1, bank.size() - 0);


	return
		uint64_t(bank[i11]) * 100000000000U +
		uint64_t(bank[i10]) * 10000000000U +
		uint64_t(bank[i9])  * 1000000000U +
		uint64_t(bank[i8])  * 100000000 +
		uint64_t(bank[i7])  * 10000000 +
		uint64_t(bank[i6])  * 1000000 +
		uint64_t(bank[i5])  * 100000 +
		uint64_t(bank[i4])  * 10000 +
		uint64_t(bank[i3])  * 1000 +
		uint64_t(bank[i2])  * 100 +
		uint64_t(bank[i1])  * 10 +
		uint64_t(bank[i0])  * 1;
}

uint64_t sum_joltage_part_2(std::string const& name)
{
	uint64_t result = 0;

	MemoryMappedFile file(name);

	for_each_line(
		file.get(),
		[&result](std::string_view line){
			auto const jolts = get_max_joltage_part_2(make_bank(line));
			result += jolts;
			// fmt::println("line: {} -> {}", line, jolts);
		});

	return result;
}

TEST_CASE("day 3-1")
{
	REQUIRE(sum_joltage_part_1("./input/day3/example.txt") == 357);
	REQUIRE(sum_joltage_part_1("./input/day3/input.txt") == 16927);
}

TEST_CASE("day 3-2")
{
	REQUIRE(sum_joltage_part_2("./input/day3/example.txt") == 3121910778619);
	REQUIRE(sum_joltage_part_2("./input/day3/input.txt") == 167384358365132);
}
