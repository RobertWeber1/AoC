#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>
#include <array>


const MemoryMappedFile example(FileDescriptor("./input/day4/example.txt"));
const MemoryMappedFile input(FileDescriptor("./input/day4/input.txt"));

struct Shape
{
	int rows;
	int colums;
};

Shape get_shape(std::string_view data)
{
	Shape result = {0, 0};
	for(int i = 0; i<data.size(); ++i)
	{
		printf("%02x ", data[i]);
		if(data[i] == '\n')
		{
			result.colums = i;
			result.rows = data.size() / (result.colums);
			break;
		}
	}
	puts("");
	for(int j = result.colums; j<data.size(); j+=(result.colums+1))
	{
	 	if(data[j] != '\n')
		{
			printf("value(%d) is %02x but should be %02x, \n", j, data[j], '\n');
			throw std::logic_error("input is misshaped!");
		}
	// 	result.rows = j;
	}

	return result;
}

int count_xmas(std::string_view data)
{
	std::array<std::string_view, 7> kernel = {
	    "s  s  s"sv,
		" a a a "sv,
		"  mmm  "sv,
		"smaxmas"sv,
		"  mmm  "sv,
		" a a a "sv,
		"s  s  s"sv};
	auto const shape = get_shape(data);
	return shape.rows;
}

TEST_CASE("day 4")
{
	REQUIRE(count_xmas(example.get()) == 18);
}
