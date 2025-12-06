#include <catch2/catch_test_macros.hpp>
#include "utils.h"

uint64_t calculate_grand_total(std::string const& name)
{
	MemoryMappedFile file(name);

	std::vector<std::vector<uint64_t>> numbers;
	std::vector<char> operations;

	for_each_line(
		file.get(),
		[&numbers, &operations](std::string_view line){
			std::vector<uint64_t> numbers_of_line;
			for_each_int(
				line,
				" "sv,
				[&numbers_of_line](uint64_t i){
					if(i != 0)
					{
						numbers_of_line.push_back(i);
					}
				});

			if(not numbers_of_line.empty())
			{
				numbers.push_back(std::move(numbers_of_line));
				return;
			}

			for_each_line(
				line,
				[&operations](std::string_view op){
					operations.push_back(op[0]);
				},
				" "sv);
		});

	uint64_t result = 0;

	for(int i = 0; i<operations.size(); ++i)
	{
		if(operations[i] == '+')
		{
			uint64_t tmp = 0;
			for(auto const& number_line : numbers)
			{
				tmp += number_line[i];
			}
			result += tmp;
		}
		else if(operations[i] == '*')
		{
			uint64_t tmp = 1;
			for(auto const& number_line : numbers)
			{
				tmp *= number_line[i];
			}
			result += tmp;
		}
	}

	return result;
}


uint64_t calculate_grand_total_part_2(std::string const& name)
{
	MemoryMappedFile file(name);


	std::vector<std::string_view> lines;
	for_each_line(
		file.get(),
		[&lines](std::string_view line){
			lines.push_back(line);
		});


	std::vector<char> operations;
	std::vector<std::vector<uint64_t>> numbers =
		std::vector<std::vector<uint64_t>>(1, std::vector<uint64_t>{});

	int current_group = 0;
	for(int i = 0; i<lines[0].size(); ++i)
	{
		uint64_t current_number = 0;
		for(int l = 0; l < lines.size(); ++l)
		{
			if(i>=lines[l].size())
			{
				continue;
			}

			if(lines[l][i] == ' ')
			{
				continue;
			}

			if(lines[l][i] == '*' or lines[l][i] == '+')
			{
				operations.push_back(lines[l][i]);
				continue;
			}

			current_number = current_number * 10 + (lines[l][i] - '0');
		}

		if(current_number != 0)
		{
			numbers[current_group].push_back(current_number);
		}
		else
		{
			current_group++;
			numbers.push_back( std::vector<uint64_t>{});
		}
	}

	uint64_t result = 0;

	for(int i = 0; i<operations.size(); ++i)
	{
		if(operations[i] == '+')
		{
			uint64_t tmp = 0;
			for(auto const& number_line : numbers[i])
			{
				tmp += number_line;
			}
			result += tmp;
		}
		else if(operations[i] == '*')
		{
			uint64_t tmp = 1;
			for(auto const& number_line : numbers[i])
			{
				tmp *= number_line;
			}
			result += tmp;
		}
	}

	return result;
}

TEST_CASE("day 6-1")
{
	REQUIRE(calculate_grand_total("./input/day6/example.txt") == 4277556);
	REQUIRE(calculate_grand_total("./input/day6/input.txt") == 6957525317641);
}


TEST_CASE("day 6-2")
{
	REQUIRE(calculate_grand_total_part_2("./input/day6/example.txt") == 3263827);
	REQUIRE(calculate_grand_total_part_2("./input/day6/input.txt") == 13215665360076);
}
