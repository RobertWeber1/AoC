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
	std::vector<Point> points;
	std::vector<Point> corners;
	size_t area = 0;
	size_t border = 0;
	size_t shitty_corner_counter = 0;
	void add_corner(Point c, bool force)
	{
		if(not force)
		{
			for(auto const& corner : corners)
			{
				if(corner == c)
				{
					return;
				}
			}
		}
		corners.push_back(c);
	}
};

/*
.....
.....
..X..
.X...
.....
*/


/*
.....
..X..
.XXX.
..X..
.....
*/

bool is_different(MultiStringView input, Point current, char plant_type)
{
	if(current.y < 0 or current.y >= input.size())
	{
		return true;
	}

	if(current.x < 0 or current.x >= input[current.y].size())
	{
		return true;
	}

	if(input[current] != plant_type)
	{
		return true;
	}

	return false;
}



int map_region(
	MultiStringView input,
	Point current,
	std::vector<std::vector<bool>> & visited_plots,
	Region & region)
{
	if(is_different(input, current, region.plant_type))
	{
		return 1;
	}

	if(visited_plots[current.y][current.x])
	{
		return 0;
	}

	bool top          = is_different(input, current + Distance( 0, -1), region.plant_type);
	bool top_right    = is_different(input, current + Distance( 1, -1), region.plant_type);
	bool right        = is_different(input, current + Distance( 1,  0), region.plant_type);
	bool bottom_right = is_different(input, current + Distance( 1,  1), region.plant_type);
	bool bottom       = is_different(input, current + Distance( 0,  1), region.plant_type);
	bool bottom_left  = is_different(input, current + Distance(-1,  1), region.plant_type);
	bool left         = is_different(input, current + Distance(-1,  0), region.plant_type);
	bool top_left     = is_different(input, current + Distance(-1, -1), region.plant_type);

	if((left /*and top_left*/ and top) or (not left and top_left and not top))
		region.add_corner(current, left and not top_left and top);
	if((right /*and top_right*/ and top) or (not right and top_right and not top))
		region.add_corner(current+Distance(1,0), right and not top_right and top);
	if((right /*and bottom_right*/ and bottom) or (not right and bottom_right and not bottom))
		region.add_corner(current+Distance(1,1), right and not bottom_right and bottom);
	if((left /*and bottom_left*/ and bottom) or (not left and bottom_left and not bottom))
		region.add_corner(current+Distance(0,1), left and not bottom_left and bottom);


	visited_plots[current.y][current.x] = true;
	region.points.push_back(current);
	++region.area;
	region.border += map_region(input, current + Distance( 0, -1), visited_plots, region);
	region.border += map_region(input, current + Distance( 1,  0), visited_plots, region);
	region.border += map_region(input, current + Distance( 0,  1), visited_plots, region);
	region.border += map_region(input, current + Distance(-1,  0), visited_plots, region);

	return 0;
}

uint64_t calc_fence_price(MultiStringView input, Part part)
{
	std::vector<std::vector<bool>> visited_plots(
		input.size(),
		std::vector<bool>(input[0].size(), false));

	std::vector<Region> regions;

	for(int y = 0; y < input.size(); ++y)
	{
		for(int x = 0; x < input[y].size(); ++x)
		{
			auto const pos = Point(x, y);
			if(visited_plots[y][x])
			{
				continue;
			}
			regions.push_back(Region{});
			regions.back().plant_type = input[pos];
			map_region(input, pos, visited_plots, regions.back());
			// fmt::println("region {}, area: {}, corners: {}", regions.back().plant_type, regions.back().area, regions.back().corners);
		}
	}

	if(part == Part::one)
	{
		return std::ranges::fold_left(
			regions,
			0,
			[](int sum, Region const& region)
			{
				return sum + region.area * region.border;
			});
	}

	return std::ranges::fold_left(
		regions,
		0,
		[](int sum, Region const& region)
		{
			return sum + region.area *
				(region.corners.size() + region.shitty_corner_counter);
		});
}

TEST_CASE("day 12")
{
	SECTION("part 1")
	{
		REQUIRE(calc_fence_price(day12::Example::data1, Part::one) == 140);
		REQUIRE(calc_fence_price(day12::Example::data2, Part::one) == 772);
		REQUIRE(calc_fence_price(day12::Example::data3, Part::one) == 1930);
		REQUIRE(calc_fence_price(day12::Input::data, Part::one) == 1319878);
	}

	SECTION("part 2")
	{
		REQUIRE(calc_fence_price(day12::Example::data1, Part::two) == 80);
		REQUIRE(calc_fence_price(day12::Example::data2, Part::two) == 436);
		REQUIRE(calc_fence_price(day12::Example::data3, Part::two) == 1206);
		REQUIRE(calc_fence_price(day12::Example::data4, Part::two) == 236);
		REQUIRE(calc_fence_price(day12::Example::data5, Part::two) == 368);
		REQUIRE(calc_fence_price(day12::Input::data, Part::two) == 784982);
	}
}

