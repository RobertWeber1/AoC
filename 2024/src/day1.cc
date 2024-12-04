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

#include <day1/example.h>
#include <day1/input.h>

using namespace std::literals;

constexpr int day1func(std::string_view const& input, Part part)
{
	std::vector<int> numbers[2];

	int index = 0;
	for (auto && line :
		input
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

constexpr int use_data(std::string_view const& data)
{
	std::vector<char> foo;
	foo.resize(data.size());

	return foo.capacity();
}



TEST_CASE("day 1")
{
	SECTION("part 1")
	{
		AOC_REQUIRE(day1func(day1::Example::data, Part::one) == 11);
		AOC_REQUIRE(day1func(day1::Input::data, Part::one) == 1873376);
	}

	SECTION("part 2")
	{
		AOC_REQUIRE(day1func(day1::Example::data, Part::two) == 31);
		AOC_REQUIRE(day1func(day1::Input::data, Part::two) == 18997088);
	}
}
