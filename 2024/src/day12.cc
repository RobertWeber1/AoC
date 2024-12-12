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
#include <point.h>
#include <variant>

#include <day12/example.h>
#include <day12/input.h>

struct Region
{
	char plant_type;
	std::vector<Point> border;
	size_t area;
};



void map_region(
	MultiStringView input,
	Point start,
	std::vector<std::vector<bool>> & visited_plots,
	std::vector<Region> & regions)
{
	Region region;
	region.plant_type = input[start];

	visited_plots[start.y][start.x] = true
	if(input[current] == region.plant_type)
	{

	}
	auto const east = Direction(1, 0);
	auto const next = current + east;

}

uint64_t calc_fence_price(MultiStringView input)
{
	std::vector<std::vector<bool>> visited_plots(
		input.size(),
		std::vector<bool>(input[0].size(), false));

	std::vector<Region> regions;

	for(int y = 0; y < input.size(); ++y)
	{
		for(int x = 0; x < input[y].size(); ++x)
		{
			fmt::println("input[{}][{}] {}", y, x, input[y][x]);
			if(visited_plots[y][x])
			{
				continue;
			}

			map_region(input, Point(x, y), visited_plots, regions);
		}
		fmt::println("");
	}
	return 0;
}

TEST_CASE("day 12")
{
	SECTION("part 1")
	{
		REQUIRE(calc_fence_price(day12::Example::data1) == 140);
	}

	SECTION("part 2")
	{

	}
}

