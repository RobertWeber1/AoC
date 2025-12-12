#include <catch2/catch_test_macros.hpp>
#include "utils.h"
#include "point.h"

std::vector<Point> get_points(std::string_view input)
{
	std::vector<Point> points;
	for_each_line(
		input,
		[&points](std::string_view line)
		{
			std::vector<uint64_t> tmp;
			for_each_int(line, ",", [&tmp](uint64_t i){tmp.push_back(i);});
			points.push_back(to_point(tmp));
		});

	return points;
}


uint64_t get_largest_red_square(std::string name)
{
	MemoryMappedFile file(name);

	std::vector<Point> points = get_points(file.get());

	uint64_t result = 0;
	for(int i=0; i<points.size()-1; ++i)
	{
		for(int j=i+1; j<points.size(); ++j)
		{
			auto const& a = points[i];
			auto const& b = points[j];

			uint64_t width = 1 + ((a.x > b.x) ? (a.x - b.x) : (b.x - a.x));
			uint64_t height = 1 + ((a.y > b.y) ? (a.y - b.y) : (b.y - a.y));
			uint64_t size = width * height;

			if(size > result)
			{
				result = size;
			}
		}
	}

	return result;
}



void draw_line(
	Point const& p1,
	Point const& p2,
	std::vector<std::vector<char>> & tile_map)
{
	bool is_vertical = p1.x == p2.x;
	if(is_vertical)
	{
		int start = p1.y;
		int end = p2.y;
		if(p1.y > p2.y)
		{
			start = p2.y;
			end = p1.y;
		}
		for(int i = start; i<end+1; ++i)
		{
			tile_map[i][p1.x] = '#';
		}
	}
	else
	{
		int start = p1.x;
		int end = p2.x;
		if(p1.x > p2.x)
		{
			start = p2.x;
			end = p1.x;
		}
		for(int i = start; i<end+1; ++i)
		{
			tile_map[p1.y][i] = '#';
		}
	}
}

void fill_area_around(Point const& p, std::vector<std::vector<char>> & tile_map)
{
	// xmax: 98364, ymax: 98329, last: Point(98198, 50248)
	// Point(98101, 50332)

	// fmt::println("{}", p);
	tile_map[p.y][p.x] = '#';
	auto xp1 =  p.x+1;
	if(xp1 < tile_map[0].size())
	{
		if(tile_map[p.y][xp1] != '#')
		{
			fill_area_around(Point{xp1, p.y}, tile_map);
		}
	}

	auto xm1 =  p.x-1;
	if(xm1 >= 0)
	{
		if(tile_map[p.y][xm1] != '#')
		{
			fill_area_around(Point{xm1, p.y}, tile_map);
		}
	}

	auto yp1 =  p.y+1;
	if(yp1 < tile_map.size())
	{
		if(tile_map[yp1][p.x] != '#')
		{
			fill_area_around(Point{p.x, yp1}, tile_map);
		}
	}

	auto ym1 =  p.y-1;
	if(ym1 >= 0)
	{
		if(tile_map[ym1][p.x] != '#')
		{
			fill_area_around(Point{p.x, ym1}, tile_map);
		}
	}
}

bool all_points_on_line_are(
	Point const& a,
	Point const& b,
	char c,
	std::vector<std::vector<char>> & tile_map)
{
	for(int y = std::min(a.y, b.y); y < std::min<int>(std::max(a.y, b.y) + 1, tile_map.size()); ++y)
	{
		for(int x = std::min(a.x, b.x); x < std::min<int>(std::max(a.x, b.x) + 1, tile_map[0].size()); ++x)
		{
			if(tile_map[y][x] != c)
			{
				return true;
			}
		}
	}

	return false;
}

bool has_intersection_with(
	Point const& a,
	Point const& b,
	std::vector<std::vector<char>> & tile_map)
{
	return
		all_points_on_line_are(a, Point{a.x, b.y}, '#', tile_map) or
		all_points_on_line_are(Point{a.x, b.y}, b, '#', tile_map) or
		all_points_on_line_are(b, Point{b.x, a.y}, '#', tile_map) or
		all_points_on_line_are(Point{b.x, a.y}, a, '#', tile_map);
}


uint64_t get_largest_red_square_inside_of_hull(std::string name)
{
	MemoryMappedFile file(name);

	std::vector<Point> points = get_points(file.get());


	int xmax=std::numeric_limits<int>::min();
	int ymax=std::numeric_limits<int>::min();

	for(auto const& point : points)
	{
		xmax = std::max(point.x, xmax);
		ymax = std::max(point.y, ymax);
	}

	fmt::println("xmax: {}, ymax: {}, last: {}", xmax, ymax, points.back());

	std::vector<std::vector<char>> tile_map;
	tile_map.reserve(ymax+2);
	for(int i = 0; i<ymax+2; ++i)
	{
		tile_map.push_back(std::vector<char>(xmax+2, '.'));
	}

	for(auto const& pair : points | std::views::slide(2))
	{
		draw_line(pair[0], pair[1], tile_map);
	}
	draw_line(points.back(), points.front(), tile_map);


	fill_area_around(points.back(), tile_map);


	for(auto const& line : tile_map)
	{
		for(char c : line)
		{
			putchar(c);
		}
		putchar('\n');
	}

	uint64_t result = 0;
	for(int i=0; i<points.size()-1; ++i)
	{
		for(int j=i+1; j<points.size(); ++j)
		{
			auto const& a = points[i];
			auto const& b = points[j];

			uint64_t width = 1 + ((a.x > b.x) ? (a.x - b.x) : (b.x - a.x));
			uint64_t height = 1 + ((a.y > b.y) ? (a.y - b.y) : (b.y - a.y));
			uint64_t size = width * height;

			if(size > result and not has_intersection_with(a, b, tile_map))
			{
				result = size;
			}
		}
	}

	fmt::println("last is {}", result);
	return result;
}




TEST_CASE("day 9-1")
{
	REQUIRE(get_largest_red_square("./input/day9/example.txt") == 50);
	REQUIRE(get_largest_red_square("./input/day9/input.txt") == 4739623064);
}

TEST_CASE("day 9-2")
{
	REQUIRE(get_largest_red_square_inside_of_hull("./input/day9/example.txt") == 24);
	// REQUIRE(get_largest_red_square_inside_of_hull("./input/day9/input.txt") == 24);
}
