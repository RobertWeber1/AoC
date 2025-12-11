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
#include <limits>

#include <day15/example.h>
#include <day15/example2.h>
#include <day15/input.h>


uint64_t sum_gps_coordinates(std::vector<std::vector<char>> const& input)
{
	uint64_t result = 0;

	for(size_t y=1; y<input.size()-1; ++y)
	{
		for(size_t x=1; x<input[0].size()-1; ++x)
		{
			if(input[y][x] == 'O')
			{
				result += (x + y) * 100;
			}
		}
	}
	return result;
}

void move_direction(Point & pos, Distance dir, std::vector<std::vector<char>> & map)
{
	bool can_move = true
	Point next = robot_pos + dir;
	while(can_move)
	{
		auto & next_c = map[next.y][next.x];
		if(next_c == '.')
		{
			std::swap(next_c, map[robot_pos.y][robot_pos.x])
		}

		next = next + dir;
	}
}

std::vector<std::vector<char>> apply_robot_commands(
	MultiStringView input,
	std::string_view commands)
{
	std::vector<std::vector<char>> result;
	Point robot_pos;
	for(size_t y=0; y<input.size(); ++y)
	{
		result.push_back(std::vector<char>{});
		for(size_t x=0; x<input[0].size(); ++x)
		{
			result.back().push_back(input[y][x]);
			if(input[y][x] == '@')
			{
				robot_pos = Point(x, y);
			}
		}
	}

	fmt::println("robot pos: {}", robot_pos);

	for(char c : commands)
	{
		if(c == '<')
		{
			move_direction(robot_pos, left(), result);
		}
		else if(c == '>')
		{
			move_direction(robot_pos, right(), result);
		}
		else if(c == '^')
		{
			move_direction(robot_pos, up(), result);
		}
		else if(c == 'v')
		{
			move_direction(robot_pos, down(), result);
		}
	}

	return result;
}

TEST_CASE("day 15")
{
	SECTION("part 1")
	{
		REQUIRE(
			sum_gps_coordinates(
				apply_robot_commands(
					day15::Example::map,
					day15::Example::commands))
			== 2028);

		REQUIRE(
			sum_gps_coordinates(
				apply_robot_commands(
					day15::Example2::map,
					day15::Example2::commands))
			== 10092);

		REQUIRE(
			sum_gps_coordinates(
				apply_robot_commands(
					day15::Input::map,
					day15::Input::commands))
			== 10092);
	}

	SECTION("part 2")
	{
	}
}

