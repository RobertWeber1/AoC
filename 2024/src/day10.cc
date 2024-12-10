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
#include <day10/example.h>
#include <day10/input.h>


struct Trail
{
	Point start;
	std::vector<Point> end;
	int rating = 0;
	size_t get_score() const
	{
		return end.size();
	}
};

template<size_t N>
std::vector<Trail> get_initial_trails(std::string_view const (&data)[N])
{
	std::vector<Trail> trails;

	for(int y = 0; y < N; ++y)
	{
		for(int x = 0; x < data[y].size(); ++x)
		{
			if(data[y][x] == '0')
			{
				trails.emplace_back(Point{x,y}, std::vector<Point>{});
			}
		}
	}

	return trails;
}

struct HeightPoint
{
	char height;
	Point pos;
};

template<size_t N>
HeightPoint get_height(std::string_view const (&data)[N], Point pos)
{
	if(pos.y >= 0 and pos.y < N)
	{
		if(pos.x >= 0 and pos.x < data[pos.y].size())
		{
			return {data[pos.y][pos.x], pos};
		}
	}
	return {'0' - 1, pos};
}

void add_if_appropriate(
	std::vector<HeightPoint> & out,
	HeightPoint const& hp,
	char height)
{
	if(hp.height - height == 1)
		out.push_back(hp);
}

template<size_t N>
std::vector<HeightPoint> get_next(std::string_view const (&data)[N], Point pos)
{
	char height = get_height(data, pos).height;

	Point north = pos + Distance(0, -1);
	Point east  = pos + Distance(1, 0);
	Point south = pos + Distance(0, 1);
	Point west  = pos + Distance(-1, 0);

	std::vector<HeightPoint> directions;

	add_if_appropriate(directions, get_height(data, north), height);
	add_if_appropriate(directions, get_height(data, east), height);
	add_if_appropriate(directions, get_height(data, south), height);
	add_if_appropriate(directions, get_height(data, west), height);

	return directions;
}



template<size_t N>
void find_end(
	std::string_view const (&data)[N],
	Trail & trail,
	HeightPoint const& current)
{
	if(current.height == '9')
	{
		for(auto const& point : trail.end)
		{
			if(point == current.pos)
			{
				// fmt::println("dupplicate end: {}", current.pos);
				++trail.rating;
				return;
			}
		}
		// fmt::println("new end: {}", current.pos);
		++trail.rating;
		trail.end.push_back(current.pos);
		return;
	}

	auto dirs = get_next(data, current.pos);

	for(auto const& dir : dirs)
	{
		// fmt::println("    {} - {}", dir.height, dir.pos);
		find_end(data, trail, dir);
	}
}

template<size_t N>
std::vector<Trail> find_paths(
	std::string_view const (&data)[N],
	std::vector<Trail> trails)
{
	for(auto & trail : trails)
	{
		// fmt::println("start {}", trail.start);

		find_end(data, trail, {'0', trail.start});
		// fmt::println("-----------------{}----------------------\n", trail.end.size());
	}

	return trails;
}



int sum_scores(std::vector<Trail> const& trails)
{
	return std::ranges::fold_left(
		trails,
		0,
		[](int sum, Trail const& t)
		{
			return sum + t.get_score();
		});
}



int sum_ratings(std::vector<Trail> const& trails)
{
	return std::ranges::fold_left(
		trails,
		0,
		[](int sum, Trail const& t)
		{
			return sum + t.rating;
		});
}



TEST_CASE("day 10")
{
	SECTION("part 1")
	{
		REQUIRE(
			sum_scores(
				find_paths(
					day10::Example::data,
					get_initial_trails(day10::Example::data)))
			== 36);

		REQUIRE(
			sum_scores(
				find_paths(
					day10::Input::data,
					get_initial_trails(day10::Input::data)))
			== 607);
	}

	SECTION("part 2")
	{
		REQUIRE(
			sum_ratings(
				find_paths(
					day10::Example::data,
					get_initial_trails(day10::Example::data)))
			== 81);

		REQUIRE(
			sum_ratings(
				find_paths(
					day10::Input::data,
					get_initial_trails(day10::Input::data)))
			== 1384);
	}
}
