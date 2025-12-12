#include <catch2/catch_test_macros.hpp>
#include "utils.h"
#include <map>

using Graph_t = std::map<std::string, std::vector<std::string>>;
using Lookup_t = std::map<std::string, uint64_t>;

Graph_t get_graph(std::string name)
{
	MemoryMappedFile file(name);

	Graph_t graph;

	for_each_line(
		file.get(),
		[&graph](std::string_view line)
		{
			std::string key;
			std::vector<std::string> edges;
			for_each_line(
				line,
				[&key, &edges](std::string_view part)
				{
					if(key == "")
					{
						key = part.substr(0, part.size()-1);
					}
					else
					{
						edges.push_back(std::string(part));
					}

				}, " ");

			graph.emplace(key, std::move(edges));
		});

	return graph;
}



uint64_t count_paths_from_to(std::string const& start, std::string const& end, Graph_t const& graph, Lookup_t & lookup)
{
	uint64_t result = 0;

	auto precalced = lookup.find(start);
	if(precalced != lookup.end())
	{
		return precalced->second;
	}

	auto const& nodes = graph.find(start);
	if(nodes == graph.end())
	{
		return 0;
	}
	for(auto const& node : nodes->second)
	{
		if(node == end)
		{
			return 1;
		}
		else
		{
			result += count_paths_from_to(node, end, graph, lookup);
		}
	}

	lookup[start] = result;

	return result;
}

uint64_t count_paths(std::string const& start, std::string const& end, Graph_t const& graph)
{
	Lookup_t fft_to_dac_lookup;
	return count_paths_from_to(start, end, graph, fft_to_dac_lookup);
}

uint64_t count_paths_over_dac_fft(Graph_t const& graph)
{
	return
		count_paths("svr", "fft", graph) * count_paths("fft", "dac", graph) * count_paths("dac", "out", graph) +
		count_paths("svr", "dac", graph) * count_paths("dac", "fft", graph) * count_paths("fft", "out", graph);
}


TEST_CASE("day 11-1")
{
	REQUIRE(count_paths("you", "out", get_graph("./input/day11/example.txt")) == 5);
	REQUIRE(count_paths("you", "out", get_graph("./input/day11/input.txt")) == 539);
}

TEST_CASE("day 11-2")
{
	REQUIRE(count_paths_over_dac_fft(get_graph("./input/day11/example2.txt")) == 2);
	REQUIRE(count_paths_over_dac_fft(get_graph("./input/day11/input.txt")) == 413167078187872);
}

