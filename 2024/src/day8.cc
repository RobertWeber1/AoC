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

#include <day8/example.h>
#include <day8/input.h>

struct Point
{
	int x;
	int y;
};

struct Distance
{
	int x;
	int y;
};

Distance operator-(Point const& lhs, Point const& rhs)
{
	return Distance{rhs.x-lhs.x, rhs.y-lhs.y};
}

Point operator+(Point const& lhs, Distance const& rhs)
{
	return Point{rhs.x+lhs.x, rhs.y+lhs.y};
}

Point operator-(Point const& lhs, Distance const& rhs)
{
	return Point{rhs.x-lhs.x, rhs.y-lhs.y};
}

bool operator==(Point const& lhs, Point const& rhs)
{
	return lhs.x==rhs.x and lhs.y==rhs.y;
}

bool operator<(Point const& lhs, Point const& rhs)
{
	return lhs.x<rhs.x and lhs.y<rhs.y;
}

template <>
class fmt::formatter<Point>
{
public:
	constexpr auto parse (format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template <typename Context>
	constexpr auto format (Point const& point, Context& ctx) const
	{
		return fmt::format_to(ctx.out(), "Point({}, {})", point.x, point.y);
	}
};

template <>
class fmt::formatter<Distance>
{
public:
	constexpr auto parse (format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template <typename Context>
	constexpr auto format (Distance const& dist, Context& ctx) const
	{
		return fmt::format_to(ctx.out(), "Distance({}, {})", dist.x, dist.y);
	}
};
template<size_t N>
bool collides(std::string_view const (&data)[N], Point node)
{

	if((node.y < 0) or (node.y >= N))
	{
		fmt::println("    {} out of y-bounds", node);
		return true;
	}

	if((node.x < 0) or (node.x >= data[node.y].size()))
	{
		fmt::println("    {} out of x-bounds", node);
		return true;
	}

	if(data[node.y][node.x] != '.')
	{
		fmt::println("    collision @ {} with {}", node, data[node.y][node.x]);
		return true;
	}

	return false;
}

bool already_present(std::vector<Point> const& nodes, Point const& node)
{
	for(auto const& n : nodes)
	{
		if(n == node)
		{
			return true;
		}
	}
	return false;
}

int max_node_count(int i)
{
	return i * (i-1);
}

template<size_t N>
int count_anti_nodes(std::string_view const (&data)[N])
{
	int result = 0;
	const size_t max_x = data[0].size()-1;
	const size_t max_y = N-1;

	std::map<char, std::vector<Point>> antenna_positions;

	for(size_t y=0; y<N; ++y)
	{
		for(size_t x=0; x<data[y].size(); ++x)
		{
			char c = data[y][x];
			if(c != '.')
			{
				antenna_positions[c].emplace_back(x,y);
			}
		}
	}

	for(auto const& pair : antenna_positions)
	{
		fmt::println("{}: {}", pair.first, pair.second.size());
	}
	int total_count = 0;
	std::vector<Point> nodes;
	for(auto const& pair : antenna_positions)
	{
		fmt::println("----------{} * {} max_nodes: {}----------", pair.first, pair.second.size(), max_node_count(pair.second.size()));
		auto const& positions = pair.second;
		int local_count = 0;
		for(int pivot = 0; pivot < positions.size()-1; ++pivot)
		{
			for(int i=pivot+1; i<positions.size(); ++i)
			{
				fmt::println("  {} - {}", positions[pivot], positions[i]);

				Point node1 = positions[i]+(positions[pivot]-positions[i]);
				if(not collides(data, node1))
				{
					fmt::println("    add node1 {}", node1);
					++local_count;
					++total_count;
				}

				Point node2 = positions[pivot] +(positions[i]-positions[pivot]);
				if(not collides(data, node2))
				{
					fmt::println("    add node2 {}", node2);
					++local_count;
					++total_count;
				}
			}
			puts("");
		}
		fmt::println("----------{}----------", local_count);
	}

	fmt::println("total_count: {}", total_count);
	return total_count;
}


TEST_CASE("day 8")
{
	SECTION("part 1")
	{
		REQUIRE(count_anti_nodes(day8::Example::data) == 14);
		REQUIRE(count_anti_nodes(day8::Input::data) == 279);//  267);   //max value 474
	}

	SECTION("part 2")
	{
	}
}
