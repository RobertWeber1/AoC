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

#include <day14/example.h>
#include <day14/input.h>

struct Robot
{
	Point pos;
	Velocity vel;
};

std::vector<Robot> get_robots(MultiStringView input)
{
	std::vector<Robot> robots;

	for(auto line : input)
	{
		auto [foo, px, py, vx, vy] = ctre::search<"p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)">(line);

		Point pos{string_view_to_int(px.to_view()), string_view_to_int(py.to_view())};
		Velocity vel{string_view_to_int(vx.to_view()), string_view_to_int(vy.to_view())};

		robots.emplace_back(pos, vel);
	}

	return robots;
}

uint64_t calc_safety_factor(std::vector<Robot> const& input, Size size, std::chrono::seconds time)
{
	uint64_t sector_count[4] = {0};
	for(auto const& robot : input)
	{
		Point new_pos = (robot.pos + (robot.vel * time)) % size;

		if(new_pos.x < size.x/2)
		{
			if(new_pos.y < size.y/2)
			{
				++sector_count[1];
			}
			else if(new_pos.y > size.y/2)
			{
				++sector_count[2];
			}
		}
		else if(new_pos.x > size.x/2)
		{
			if(new_pos.y < size.y/2)
			{
				++sector_count[0];
			}
			else if(new_pos.y > size.y/2)
			{
				++sector_count[3];
			}
		}
	}

	return sector_count[0] * sector_count[1] * sector_count[2] * sector_count[3];
}

bool check_threshold(std::vector<Robot> const& input, Size size, std::chrono::seconds time, Size window, int factor = 50)
{
	std::vector<std::vector<char>> robot_map(window.y, std::vector<char>(window.x, '.'));
	auto mid_x = size.x/2;
	auto mid_y = size.y/2;

	auto out_mid_x = window.x/2;
	auto out_mid_y = window.y/2;

	Point upper_left  = Point{mid_x - out_mid_x, mid_y - out_mid_y};
	Point lower_right = Point{mid_x + out_mid_x, mid_y + out_mid_y};

	int count = 0;
	for(auto const& robot : input)
	{
		Point new_pos = (robot.pos + (robot.vel * time)) % size;

		if(new_pos.x>=upper_left.x and new_pos.x <= lower_right.x and
		   new_pos.y>=upper_left.y and new_pos.y <= lower_right.y)
		{
			++count;
			// auto const translated = new_pos - upper_left;
			// robot_map[translated.y][translated.x] = '#';
		}

	}

	return ((count * 100 / (window.x * window.y)) > factor) or (count == 0);
}

void print_map(std::vector<std::vector<char>> const& input)
{
	for(auto const& line : input)
	{
		for(char c : line)
		{
			fmt::print("{}", c);
		}
		fmt::println("");
	}
}

std::chrono::seconds find_easer_egg_time(std::vector<Robot> const& input, Size full_size, Size window)
{
	std::chrono::seconds time = 1s;

	while(not check_threshold(input, full_size, time, window))
	{
		++time;
		if(time == 10000000s)
		{
			break;
		}
	}

	return time;
}

TEST_CASE("Point modulo")
{
	REQUIRE(Point(-1,0) % Size(10,10) == Point(0, 0));
}

TEST_CASE("day 14")
{
	auto robots = get_robots(day14::Input::data);

	SECTION("part 1")
	{
		auto example_robots = get_robots(day14::Example::data);
		REQUIRE(calc_safety_factor(example_robots, Size(11, 7), 100s) == 12);
		REQUIRE(calc_safety_factor(robots, Size(101, 103), 100s) == 224969976);
	}

	SECTION("part 2")
	{
		REQUIRE(find_easer_egg_time(robots, Size(101, 103), Size(25, 25)) ==  1s);
	}
}

