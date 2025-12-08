#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <set>
#include "utils.h"

using JunctionBox = std::vector<int64_t>;
using Circuit_t = std::vector<JunctionBox>;

float distance(JunctionBox const& a, JunctionBox const& b)
{
	return pow(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2), 0.5);
}

std::vector<std::tuple<float, int, int>> get_sorted_distances(std::string_view input)
{
	std::vector<JunctionBox> junction_boxes;
	junction_boxes.reserve(1000);

	for_each_line(
		input,
		[&junction_boxes](std::string_view line)
		{
			JunctionBox tmp;
			for_each_int(line, ","sv, [&tmp](uint64_t i){tmp.push_back(i);});
			junction_boxes.push_back(tmp);
		});

	std::vector<std::tuple<float, int, int>> distances;
	distances.reserve(junction_boxes.size()*junction_boxes.size());

	for(int i = 0; i<junction_boxes.size()-1; i++)
	{
		for(int j = i+1; j<junction_boxes.size(); ++j)
		{
			distances.push_back(
				std::make_tuple(
					distance(junction_boxes[i], junction_boxes[j]),
					i,
					j));
		}
	}

	std::sort(
		distances.begin(),
		distances.end(),
		[](std::tuple<float, int, int> const& lhs,
		   std::tuple<float, int, int> const& rhs)
		{
			return std::get<0>(lhs) < std::get<0>(rhs);
		});

	return distances;
}

uint64_t mult_three_largest_circuits(std::string name, int connection_count)
{
	MemoryMappedFile file(name);

	auto const distances = get_sorted_distances(file.get());

	std::vector<std::set<int>> circuits;

	int count = 0;
	for(auto const & entry : distances)
	{
		auto i = std::get<1>(entry);
		auto j = std::get<2>(entry);
		std::vector<int> indices;
		for(int k = 0; k < circuits.size(); ++k)
		{
			if(circuits[k].contains(i) or circuits[k].contains(j))
			{
				indices.push_back(k);
			}

		}
		if(indices.empty())
		{
			std::set<int> next_circuit;
			next_circuit.insert(i);
			next_circuit.insert(j);
			circuits.push_back(next_circuit);
			++count;
		}
		else if(indices.size() == 1)
		{
			circuits[indices[0]].insert(i);
			circuits[indices[0]].insert(j);
			++count;
		}
		else if(indices.size() == 2)
		{
			circuits[indices[0]].insert(i);
			circuits[indices[0]].insert(j);
			circuits[indices[0]].insert(
				circuits[indices[1]].begin(),
				circuits[indices[1]].end());

			circuits[indices[1]] = circuits.back();
			circuits.pop_back();

			++count;
		}

		if(count == connection_count)
		{
			break;
		}
	}

	std::sort(circuits.begin(), circuits.end(), [](auto const& lhs, auto const& rhs){ return lhs.size() > rhs.size();});
	fmt::println("{}", circuits);

	uint64_t result = 1;
	std::ranges::for_each(
		circuits | std::ranges::views::take(3),
		[&result](auto const& circuit)
		{
			result *= circuit.size();
		});

	return result;
}

uint64_t mult_x_of_last_connecting_pair(std::string name)
{
	MemoryMappedFile file(name);

	std::vector<JunctionBox> junction_boxes;
	junction_boxes.reserve(1000);

	for_each_line(
		file.get(),
		[&junction_boxes](std::string_view line)
		{
			JunctionBox tmp;
			for_each_int(line, ","sv, [&tmp](uint64_t i){tmp.push_back(i);});
			junction_boxes.push_back(tmp);
		});


	std::vector<std::tuple<float, int, int>> distances;
	distances.reserve(junction_boxes.size()*junction_boxes.size());

	for(int i = 0; i<junction_boxes.size()-1; i++)
	{
		for(int j = i+1; j<junction_boxes.size(); ++j)
		{
			distances.push_back(
				std::make_tuple(
					distance(junction_boxes[i], junction_boxes[j]),
					i,
					j));
		}
	}

	std::sort(
		distances.begin(),
		distances.end(),
		[](std::tuple<float, int, int> const& lhs,
		   std::tuple<float, int, int> const& rhs)
		{
			return std::get<0>(lhs) < std::get<0>(rhs);
		});

	std::vector<std::set<int>> circuits;

	for(auto const & entry : distances)
	{
		auto i = std::get<1>(entry);
		auto j = std::get<2>(entry);
		std::vector<int> indices;
		for(int k = 0; k < circuits.size(); ++k)
		{
			if(circuits[k].contains(i) or circuits[k].contains(j))
			{
				indices.push_back(k);
			}

		}
		if(indices.empty())
		{
			std::set<int> next_circuit;
			next_circuit.insert(i);
			next_circuit.insert(j);
			circuits.push_back(next_circuit);
		}
		else if(indices.size() == 1)
		{

			circuits[indices[0]].insert(i);
			circuits[indices[0]].insert(j);
			if(circuits[indices[0]].size() == junction_boxes.size())
			{
				fmt::println("last connection between {} and {}", junction_boxes[i], junction_boxes[j]);
				return junction_boxes[i][0] * junction_boxes[j][0];
			}
		}
		else if(indices.size() == 2)
		{

			circuits[indices[0]].insert(i);
			circuits[indices[0]].insert(j);
			circuits[indices[0]].insert(
				circuits[indices[1]].begin(),
				circuits[indices[1]].end());


			if(circuits[indices[0]].size() == junction_boxes.size())
			{
				fmt::println("last connection between {} and {}", junction_boxes[i], junction_boxes[j]);
				return junction_boxes[i][0] * junction_boxes[j][0];
			}

			circuits[indices[1]] = circuits.back();
			circuits.pop_back();
		}
	}

	fmt::println("should not end here!!!!!!!!! {}", circuits);
	return 0;
}


TEST_CASE("day 8-1")
{
	REQUIRE(mult_three_largest_circuits("./input/day8/example.txt", 10) == 40);
	REQUIRE(mult_three_largest_circuits("./input/day8/input.txt", 1000) == 67488);
}

TEST_CASE("day 8-2")
{
	REQUIRE(mult_x_of_last_connecting_pair("./input/day8/example.txt") == 25272);
	REQUIRE(mult_x_of_last_connecting_pair("./input/day8/input.txt") == 3767453340);
}
