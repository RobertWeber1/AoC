#include <catch2/catch_test_macros.hpp>
#include "utils.h"

struct IngredientIDRange
{
	uint64_t begin;
	uint64_t end;

	constexpr bool operator<(IngredientIDRange const& other) const
	{
		return begin < other.begin;
	}

	constexpr bool contains(uint64_t id) const
	{
		return id >= begin and id <= end;
	}
};

std::vector<IngredientIDRange> get_ranges(std::string_view input)
{
	std::vector<IngredientIDRange> id_ranges;

	for_each_line(
		input,
		[&id_ranges](std::string_view raw_range)
		{
			std::vector<uint64_t> ints;
			for_each_int(
				raw_range,
				"-"sv,
				[&ints](uint64_t i){ints.push_back(i);});
			id_ranges.push_back(IngredientIDRange{ints[0], ints[1]});

			std::sort(
				id_ranges.begin(),
				id_ranges.end(),
				[](IngredientIDRange const& lhs, IngredientIDRange const& rhs){
					return lhs < rhs;
				});
		});



	return id_ranges;
}

uint64_t count_fresh_ingredients(std::string const& name)
{
	uint64_t result = 0;

	MemoryMappedFile file(name);

	auto split_pos = file.get().find("\n\n");

	std::string_view raw_ranges(file.get().data(), split_pos);
	std::string_view raw_ingredients(file.get().data() + split_pos + 2,  file.get().size() - split_pos);


	std::vector<IngredientIDRange> id_ranges =
		get_ranges(std::string_view(file.get().data(), split_pos));

	for_each_int(
		raw_ingredients,
		"\n"sv,
		[&id_ranges, &result](uint64_t id){
			for(auto const& range : id_ranges)
			{
				if(range.contains(id))
				{
					++result;
					return;
				}
			}
		});

	return result;
}


uint64_t fresh_ingreient_count(std::string const& name)
{
	MemoryMappedFile file(name);

	auto split_pos = file.get().find("\n\n");

	std::vector<IngredientIDRange> id_ranges =
		get_ranges(std::string_view(file.get().data(), split_pos));

	uint64_t result = 0;
	uint64_t prev_end = 0;
	for(auto const& range : id_ranges)
	{
		if(range.begin < prev_end)
		{
			if(range.end < prev_end)
			{
				continue;
			}
			auto const value = (range.end - prev_end) + 1;
			result += value;
		}
		else
		{
			auto const value = (range.end - range.begin) + 1;
			result += (range.end - range.begin) + 1;
		}


		prev_end = range.end + 1;
	}

	return result;
}

TEST_CASE("day 5-1")
{
	REQUIRE(count_fresh_ingredients("./input/day5/example.txt") == 3);
	REQUIRE(count_fresh_ingredients("./input/day5/input.txt") == 821);
}

TEST_CASE("day 5-2")
{
	REQUIRE(fresh_ingreient_count("./input/day5/example.txt") == 14);
	REQUIRE(fresh_ingreient_count("./input/day5/input.txt") == 344771884978261);
}
