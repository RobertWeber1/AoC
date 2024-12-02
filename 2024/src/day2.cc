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
#include <cmath>
#include <vector>
#include <fmt/format.h>
#include <fmt/ranges.h>

const MemoryMappedFile example(FileDescriptor("./input/day2/example.txt"));
const MemoryMappedFile data(FileDescriptor("./input/day2/input.txt"));

template<class Range>
bool is_save_report(Range && report)
{
	bool safe = true;
	bool first = true;
	int prev_div = 0;
	std::ranges::find_if(
		report
		| std::ranges::views::slide(2),
		[&safe, &first, &prev_div](auto && sub)
		{
			int a = *sub.begin();
 			int b = *(++sub.begin());

			int diff = a - b;

			if(std::signbit(prev_div) != std::signbit(diff) and not first)
			{
				safe = false;
				return true;
			}

			if(std::abs(diff) == 0 or std::abs(diff) > 3)
			{
				safe = false;
				return true;
			}

			prev_div = diff;
			first = false;

			return false;
		});

	return safe;
}


std::vector<int> remove_element(std::vector<int> in, int i)
{
	in.erase(in.begin() + i, in.begin() + i + 1);
	return in;
}


int count_safe_reports(std::string_view input, Part part)
{
	int count = 0;
	for_each_line(
		input,
		[&count, part](std::string_view line)
		{
			std::vector<int> report;
			std::ranges::for_each(
				line
				| make_splitter(" "sv)
				| std::ranges::views::transform(string_view_to_int),
				[&report](int a)
				{
					report.push_back(a);
				});

			auto safe = is_save_report(report);
			count += safe;

			if(not safe and part == Part::two)
			{
				for(int skip = 0; skip < report.size(); ++skip)
				{
					safe = is_save_report(remove_element(report, skip));

					if(safe)
						break;
				}
				count += safe;
			}
		});

	return count;
}


TEST_CASE("day 2")
{
	REQUIRE(count_safe_reports(example.get(), Part::one) == 2);
	REQUIRE(count_safe_reports(data.get(), Part::one) == 463);

	REQUIRE(count_safe_reports(example.get(), Part::two) == 4);
	REQUIRE(count_safe_reports(data.get(), Part::two) == 514);
}
