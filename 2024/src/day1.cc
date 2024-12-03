#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>
#include <charconv>
#include <map>
#include <algorithm>

using namespace std::literals;



int day1func(MemoryMappedFile const& f, Part part)
{
	std::map<int, std::vector<int>> numbers;

	int index = 0;
	for (auto && line :
		f.get()
		| std::ranges::views::split("\n"sv)
		| std::ranges::views::transform(
			[](auto && w)
			{
				return
					std::string_view(w.data(), w.size());
			}))
	{
		int col = 0;
		for(auto && word :
			line
			| std::ranges::views::split(" "sv)
			| std::ranges::views::transform(
				[&index](auto && w)
				{
					return std::string_view(w.data(), w.size());
				})
			| std::ranges::views::filter([](auto && w){ return w != ""sv; }))
		{
			numbers[col++].push_back(string_view_to_int(word));
		}

		index++;
	}

	std::ranges::sort(numbers[0]);
	std::ranges::sort(numbers[1]);



	switch(part)
	{
	case Part::one:
		return
			std::ranges::fold_left(
				std::views::zip_transform(
					[](auto a, auto b) { return std::abs(a - b); },
					numbers[0],
					numbers[1]),
				0,
				std::plus<int>());
	case Part::two:
		{
			int sum = 0;
			for(auto val : numbers[0])
			{
				auto iter = std::find(numbers[1].begin(), numbers[1].end(), val);
				if(iter == numbers[1].end())
				{
					continue;
				}
				int count = 0;
				while(iter != numbers[1].end() and *iter == val)
				{
					++iter;
					++count;
				}
				sum += val * count;
			}

			return sum;
		}
	}


	return 0;
}


const MemoryMappedFile example(FileDescriptor("./input/day1/example.txt"));
const MemoryMappedFile input(FileDescriptor("./input/day1/input.txt"));

TEST_CASE("day 1")
{
	SECTION("part 1")
	{
		REQUIRE(day1func(example, Part::one) == 11);

		REQUIRE(day1func(input, Part::one) == 1873376);
	}

	SECTION("part 2")
	{
		REQUIRE(day1func(example, Part::two) == 31);

		REQUIRE(day1func(input, Part::two) == 18997088);
	}
}


