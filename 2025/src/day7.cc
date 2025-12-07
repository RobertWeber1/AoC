#include <catch2/catch_test_macros.hpp>
#include "utils.h"
#include <map>

std::string make_start_line(std::string_view line)
{
	auto start_pos = line.find("S");

	std::string current_line;
	current_line.reserve(line.size());

	for(int i=0; i<line.size(); ++i)
	{
		current_line.append(".");
	}
	current_line[start_pos] = '|';

	return current_line;
}


uint64_t count_beam_splits(char const* name)
{
	MemoryMappedFile file(name);

	uint64_t result = 0;
	std::string current_line;
	for_each_line(
		file.get(),
		[&current_line, &result](std::string_view line)
		{
			if(current_line.empty())
			{
				current_line = make_start_line(line);
			}
			else
			{
				for(int i = 0; i<line.size(); ++i)
				{
					if(line[i] == '^' and current_line[i] == '|')
					{
						++result;
						current_line[i-1] = '|';
						current_line[i] = '^';
						current_line[i+1] = '|';
					}
				}
			}
		});



	return result;
}

std::string shift_left(std::string line, int pos)
{
	line[pos] = '.';
	line[pos-1] = '|';
	return line;
}

std::string shift_right(std::string line, int pos)
{
	line[pos] = '.';
	line[pos+1] = '|';
	return line;
}

std::map<std::pair<int, int>, uint64_t> cache;

uint64_t recurse_timelines(
	std::vector<std::string_view> input,
	std::string current_line,
	int index)
{
	// fmt::print(stderr, "\033[{};1H", index);
	// fmt::print(stderr, "{}", current_line);
	if(index >= input.size())
	{
		return 0;
	}

	for(int i=0; i < input[index].size(); ++i)
	{
		if(input[index][i] == '^' and current_line[i] == '|')
		{
			uint64_t value = 0;
			auto left_cached = cache.find(std::make_pair(index, i));
			if(left_cached != cache.end())
			{
				value = left_cached->second;
			}
			else
			{
				value = 1 +
					recurse_timelines(input, shift_left(current_line, i), index + 2) +
					recurse_timelines(input, shift_right(current_line, i), index + 2);
				cache[std::make_pair(index, i)] = value;
			}

			return value;
		}
	}

	return recurse_timelines(input, current_line, index + 2);
}


uint64_t count_timelines(char const* name)
{
	MemoryMappedFile file(name);

	auto const lines = get_lines(file.get());

	// fmt::print("\033[2J\033[H");
	// for(std::string_view line : lines)
	// {
	// 	fmt::println("{}", line);
	// }

	return 1 + recurse_timelines(lines, make_start_line(lines[0]), 2);
}

TEST_CASE("day 7-1")
{
	REQUIRE(count_beam_splits("./input/day7/example.txt") == 21);
	REQUIRE(count_beam_splits("./input/day7/input.txt") == 1622);
}


TEST_CASE("day 7-2")
{
	REQUIRE(count_timelines("./input/day7/example.txt") == 40);
	REQUIRE(count_timelines("./input/day7/input.txt") == 10357305916520);
}
