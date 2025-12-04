#include <catch2/catch_test_macros.hpp>
#include "utils.h"

struct Dimensions
{
	uint16_t width;
	uint16_t height;
};

Dimensions get_dimensions(std::string_view input)
{
	for(size_t i = 0; i<input.size(); ++i)
	{
		if(input[i] == '\n')
		{
			return {
				static_cast<uint16_t>(i),
				static_cast<uint16_t>(input.size()/(i+1))};
		}
	}

	throw "No newline in input file!";
}

struct Row
{
	std::vector<char> data;

	Row(uint16_t width)
	: data(width+2, '#')
	{}

	Row(std::string_view input)
	{
		data.reserve(input.size()+2);
		data.push_back('#');
		for(char c : input)
		{
			data.push_back(c);
		}
		data.push_back('#');
	}
};

struct Floor
{
	std::vector<Row> rows;
	Dimensions dim;

	Floor(std::string_view input)
	: dim(get_dimensions(input))
	{
		// fmt::println("dim: [{},{}]", dim.width, dim.height);
		rows.reserve(dim.height+2);
		rows.push_back(Row(dim.width));

		for_each_line(
			input,
			[&](std::string_view line)
			{
				rows.push_back(Row(line));
			});
		rows.push_back(Row(dim.width));
	}

	char & get(Point p)
	{
		return rows[p.y].data[p.x];
	}

	template<class Pred>
	char update_around(Point p, Pred pred)
	{
		char & current = get(p);

		if(current == '@')
		{
			uint8_t others = 0;
			char & nw = get(p+Distance{-1,-1});
			char &  n = get(p+Distance{ 0,-1});
			char & no = get(p+Distance{ 1,-1});
			char &  w = get(p+Distance{-1, 0});
			char &  o = get(p+Distance{ 1, 0});
			char & sw = get(p+Distance{-1, 1});
			char &  s = get(p+Distance{ 0, 1});
			char & so = get(p+Distance{ 1, 1});

			others += pred(nw) ? 1 : 0;
			others += pred(n)  ? 1 : 0;
			others += pred(no) ? 1 : 0;
			others += pred(w)  ? 1 : 0;
			others += pred(o)  ? 1 : 0;
			others += pred(sw) ? 1 : 0;
			others += pred(s)  ? 1 : 0;
			others += pred(so) ? 1 : 0;

			if(others < 4)
			{
				current = 'x';
			}
		}

		return current;
	}

	template<class Pred>
	uint64_t mark_removable_rolls(Pred pred)
	{
		uint64_t result = 0;

		for(uint16_t y=1; y<=dim.height; ++y)
		{
			for(uint16_t x=1; x<=dim.width; ++x)
			{
				result += (update_around(Point{x, y}, pred) == 'x') ? 1 : 0;
				// fmt::print("{}", get(Point{x, y}));
			}

			// fmt::print("\n");
		}

		return result;
	}
};

uint64_t count_moveable_rolls_part1(std::string const& name)
{
	MemoryMappedFile file(name);
	auto floor =  Floor(file.get());

	return floor.mark_removable_rolls([](char c){return (c == '@' or c == 'x');});
}

uint64_t count_moveable_rolls_part2(std::string const& name)
{
	MemoryMappedFile file(name);

	auto floor =  Floor(file.get());
	auto pred = [](char c){return (c == '@');};

	uint64_t ret_prev = 0;
	auto ret_curr = floor.mark_removable_rolls(pred);
	while(ret_prev != ret_curr)
	{
		ret_prev = ret_curr;
		ret_curr = floor.mark_removable_rolls(pred);
	}

	return ret_curr;
}

TEST_CASE("day 4-1")
{
	CHECK(count_moveable_rolls_part1("./input/day4/example.txt") == 13);
	CHECK(count_moveable_rolls_part1("./input/day4/input.txt") == 1551);
}

TEST_CASE("day 4-2")
{
	CHECK(count_moveable_rolls_part2("./input/day4/example.txt") == 43);
	CHECK(count_moveable_rolls_part2("./input/day4/input.txt") == 9784);
}
