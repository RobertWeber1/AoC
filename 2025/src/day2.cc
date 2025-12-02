#include <catch2/catch_test_macros.hpp>
#include <charconv>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "read_file.h"
#include "utils.h"



struct IdRange
{
	uint64_t begin_;
	uint64_t end_;

	constexpr uint64_t begin() const
	{
		return begin_;
	}

	constexpr uint64_t end() const
	{
		return end_;
	}

	constexpr auto get() const
	{
		return std::views::iota(begin_, end_+1);
	}
};


std::vector<IdRange> to_ranges(std::string const& name)
{
	MemoryMappedFile file(name);
	std::vector<IdRange> ranges;
	for_each_line(
		file.get(),
		[&ranges](std::string_view line){
			std::vector<uint64_t> ints;
			for_each_int(line, "-"sv, [&ints](uint64_t number){ints.push_back(number);});
			ranges.push_back({ints[0], ints[1]});
		},
		","sv);
	return ranges;
}

uint64_t sum_invalid_ids_part_1(std::string const& name)
{
	uint64_t result = 0;

	for(auto const& range : to_ranges(name))
	{
		for(uint64_t i : range.get())
		{
			char buffer[50];
			auto ret = std::to_chars(buffer, buffer + sizeof(buffer), i);

			if(ret.ec != std::errc())
			{
				continue;
			}

			auto const len = ret.ptr - buffer;

			if(len%2 != 0)
			{
				continue;
			}

			auto upper = std::string_view(buffer, len/2);;
			auto lower = std::string_view(buffer+len/2, len/2);

			if(upper == lower)
			{
				result += i;
			}
		}
	}

	return result;
}


uint64_t sum_invalid_ids_part_2(std::string const& name)
{
	uint64_t result = 0;

	for(auto const& range : to_ranges(name))
	{
		for(uint64_t i : range.get())
		{
			char buffer[50];
			auto ret = std::to_chars(buffer, buffer + sizeof(buffer), i);

			if(ret.ec != std::errc())
			{
				continue;
			}

			int const len = ret.ptr - buffer;

			auto str = std::string_view(buffer, len);
			for(int digits = 1; digits <= len/2; ++digits)
			{
				auto const chunks_pairs = str | std::views::chunk(digits) | std::views::slide(2);
				int match_counter = 0;
				for (auto pair : chunks_pairs)
				{
					if(std::ranges::equal(pair[0], pair[1]))
					{
						++match_counter;
					}
					else
					{
						break;
					}
				}

				if(match_counter == chunks_pairs.size())
				{
					result += i;
					break;
				}
			}
		}
	}

	return result;
}

TEST_CASE("day 2")
{
	REQUIRE(sum_invalid_ids_part_1("./input/day2/example.txt") == 1227775554);
	REQUIRE(sum_invalid_ids_part_1("./input/day2/input.txt") == 13919717792);

	REQUIRE(sum_invalid_ids_part_2("./input/day2/example.txt") == 4174379265);
	REQUIRE(sum_invalid_ids_part_2("./input/day2/input.txt") == 14582313461);
}
