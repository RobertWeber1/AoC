#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <ctre.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <set>

#include <point.h>

#include <day8/example.h>
#include <day8/input.h>

template<size_t N>
bool is_out_of_bounds(std::string_view const (&data)[N], Point node, char tower)
{

	if((node.y < 0) or (node.y >= N))
	{
		return true;
	}

	if((node.x < 0) or (node.x >= data[node.y].size()))
	{
		return true;
	}

	return false;
}

void print_map(std::vector<std::vector<char>> const& data, char interest)
{
	for(size_t y = 0; y < data.size(); ++y)
	{
		for(size_t x = 0; x < data[y].size(); ++x)
		{
			char c = data[y][x];
			if(c == '#')
			{
				fmt::print("{}", fmt::styled(c, fmt::bg(fmt::color::green)));
			}
			else if(c == interest)
			{
				fmt::print("{}", fmt::styled(c, fmt::bg(fmt::color::red)|fmt::fg(fmt::color::black)));
			}
			else
			{
				fmt::print("{}", c);
			}
		}
		puts("");
	}
}

int add_if_not_present(std::vector<std::vector<char>> & node_data, Point node)
{
	if(node_data[node.y][node.x] != '#')
	{
		node_data[node.y][node.x] = '#';
		return 1;
	}
	return 0;
}

template<size_t N>
int count_anti_nodes(std::string_view const (&data)[N])
{
	std::map<char, std::vector<Point>> antenna_positions;
	std::vector<std::vector<char>> node_data(
		N, std::vector<char>(data[0].size(), '.'));

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

	int total_count = 0;
	for(auto const& pair : antenna_positions)
	{
		auto const& positions = pair.second;

		for(int pivot = 0; pivot < positions.size()-1; ++pivot)
		{
			for(int i=pivot+1; i<positions.size(); ++i)
			{
				Point node1 = positions[i] + (positions[pivot] - positions[i]);
				if(not is_out_of_bounds(data, node1, pair.first))
				{
					total_count += add_if_not_present(node_data, node1);
				}

				Point node2 = positions[pivot] + (positions[i] - positions[pivot]);
				if(not is_out_of_bounds(data, node2, pair.first))
				{
					total_count += add_if_not_present(node_data, node2);
				}
			}
		}
	}
	return total_count;
}



template<size_t N>
int count_dense_anti_nodes(std::string_view const (&data)[N])
{
	std::map<char, std::vector<Point>> antenna_positions;
	std::vector<std::vector<char>> node_data(
		N, std::vector<char>(data[0].size(), '.'));

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

	int total_count = 0;
	for(auto const& pair : antenna_positions)
	{
		auto const& positions = pair.second;

		for(int pivot = 0; pivot < positions.size()-1; ++pivot)
		{
			for(int i=pivot+1; i<positions.size(); ++i)
			{
				Distance dist1 = positions[pivot] - positions[i];
				Point node1 = positions[i] + dist1;
				total_count += add_if_not_present(node_data, positions[i]);
				while(not is_out_of_bounds(data, node1, pair.first))
				{
					total_count += add_if_not_present(node_data, node1);
					node1 = node1 + dist1;
				}

				Distance dist2 = positions[i] - positions[pivot];
				Point node2 = positions[pivot] + dist2;
				total_count += add_if_not_present(node_data, positions[pivot]);
				while(not is_out_of_bounds(data, node2, pair.first))
				{
					total_count += add_if_not_present(node_data, node2);
					node2 = node2 + dist2;
				}
			}
		}
	}
	return total_count;
}


TEST_CASE("day 8")
{
	SECTION("part 1")
	{
		REQUIRE(count_anti_nodes(day8::Example::data) == 14);
		REQUIRE(count_anti_nodes(day8::Input::data) == 280);
	}

	SECTION("part 2")
	{
		REQUIRE(count_dense_anti_nodes(day8::Example::data) == 34);
		REQUIRE(count_dense_anti_nodes(day8::Input::data) == 958);
	}
}
