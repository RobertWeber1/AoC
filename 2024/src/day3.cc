#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>

const MemoryMappedFile example(FileDescriptor("./input/day3/example.txt"));
const MemoryMappedFile data(FileDescriptor("./input/day3/input.txt"));


TEST_CASE("day 3")
{

	for (auto match: ctre::range<"mul\((\\d{1}|\\d{2}|\\d{3}),(\\d{1}|\\d{2}|\\d{3})\)">(example.get())) {
		std::cout <<
			std::string_view(match.get<0>())
			<< " , " <<
			std::string_view(match.get<1>()) << "\n";
	}


	REQUIRE(false);
}
