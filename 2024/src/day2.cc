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

const MemoryMappedFile example(FileDescriptor("./input/day2/example.txt"));
const MemoryMappedFile data(FileDescriptor("./input/day2/input.txt"));


int count_safe_reports(std::string_view input, int damp_count = 0)
{
	int count = 0;
	for_each_line(input,
		[&count, &damp_count](std::string_view line)
		{
			bool ok = true;
			std::ranges::for_each(
				line
				| make_splitter(" "sv)
				| std::ranges::views::transform(string_view_to_int)
				| std::ranges::views::slide(2)
				| std::ranges::views::transform(
					[](auto && subrange)
					{
						int a = *subrange.begin();
						int b = *(++subrange.begin());
						std::cout << "initial a: " << a << ", b: " << b << std::endl;

						return b - a;
					})
				| std::ranges::views::slide(2)
				,
				[&ok, &damp_count](auto && subrange)
				{
					int a = *subrange.begin();
					int b = *(++subrange.begin());
					std::cout << "second x: " << a << ", y: " << b << std::endl;

					bool bad = false;
					if(std::abs(a) == 0 or std::abs(a) > 3)
						bad = true;

					if(std::abs(b) == 0 or std::abs(b) > 3)
						bad = true;

					if(std::signbit(a) != std::signbit(b))
						bad = true;

					if(bad and damp_count == 0)
					{
						ok = false;
					}

					if(bad and damp_count != 0)
					{
						std::cout << "discard first bad value" << std::endl;
						damp_count--;
					}
				});
			std::cout << "line:" <<  line <<  (ok?" Safe":" Unsafe") << std::endl;
			if(ok)
				count++;
		});

	return count;
}



TEST_CASE("day 2")
{
	REQUIRE(count_safe_reports(example.get()) == 2);
	// REQUIRE(count_safe_reports(data.get()) == 463);
	puts("......");
	REQUIRE(count_safe_reports(example.get(), 2) == 4);
}
