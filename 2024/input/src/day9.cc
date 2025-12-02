#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <fmt/ostream.h>
#include <set>

#include <day9/example.h>
#include <day9/input.h>

int count_files(std::string_view data)
{
	return (data.size()+1)/2;
}

struct Entry
{
	uint16_t id;
	uint8_t size;
	uint8_t remaining;
};


template <>
class fmt::formatter<Entry>
{
public:
	constexpr auto parse (format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template <typename Context>
	constexpr auto format (Entry const& value, Context& ctx) const
	{
		return fmt::format_to(ctx.out(), "Entry({}, {}, {})", value.id, value.size, value.remaining);
	}
};


std::vector<Entry> load_file_list(std::string_view data)
{
	uint16_t file_index = 0;
	bool is_file = true;
	std::vector<Entry> expanded_file_list;
	for(char c : data)
	{
		int size = c-'0';
		for(int i=0; i<size; ++i)
		{
			expanded_file_list.emplace_back((is_file ? file_index : 0), size, size-i);
		}

		if(is_file)
		{
			++file_index;
		}

		is_file = !is_file;
	}


	return expanded_file_list;
}



void print_compact(std::vector<Entry> in)
{
	for(auto const& entry : in)
	{
		fmt::print("{}", ((entry.id != 0) ? '#' : '.')  );
	}
	puts("\n---------------------------------\n");
};


std::vector<Entry> part_1_compress(std::vector<Entry> in)
{
	int first_file_size = in.begin()->size;
	auto start = in.begin() + first_file_size;
	auto end = in.end() - 1;
	while(start != end)
	{
		if(start->id == 0)
		{
			if(end->id != 0)
			{
				std::iter_swap(start, end);
				++start;
				--end;
			}
			else
			{
				--end;
			}
		}
		else
		{
			++start;
		}
	}

	return in;
}


std::vector<Entry>::iterator get_space(
	std::vector<Entry> & in,
	std::vector<Entry>::iterator max)
{
	for(auto iter = in.begin()+in[0].size; iter != max; ++iter)
	{
		if(iter->id != 0)
		{
			continue;
		}
		if(iter->remaining >= max->size)
		{
			return iter;
		}
	}

	return in.end();
}


std::vector<Entry> part_2_compress(std::vector<Entry> in)
{
	for(auto iter = in.end()-1; iter != in.begin(); --iter)
	{
		if(iter->id == 0)
		{
			continue;
		}

		auto local_end = iter - iter->size;

		auto space_iter = get_space(in, iter);
		if(space_iter == in.end())
		{
			iter = iter - iter->size +1;
			continue;
		}

		auto local_begin = iter;
		while(local_begin != local_end)
		{
			std::iter_swap(local_begin, space_iter);
			--local_begin;
			++space_iter;
		}
	}

	return in;
}

uint64_t calc_checksum(std::vector<Entry> expanded_file_list)
{
	uint64_t result = 0;
	for(size_t i = 0; i<expanded_file_list.size(); ++i)
	{
		result += (uint64_t(i) * uint64_t(expanded_file_list[i].id));
	}
	return result;
}


TEST_CASE("count_files")
{
	REQUIRE(count_files(day9::Example::data0) == 3);
	REQUIRE(count_files(day9::Example::data1) == 10);
	REQUIRE(count_files(day9::Input::data) == 10000);
}

TEST_CASE("day 9")
{
	SECTION("part 1")
	{
		REQUIRE(
			calc_checksum(
				part_1_compress(
					load_file_list(day9::Example::data1)))
			== 1928);
		REQUIRE(
			calc_checksum(
				part_1_compress(
					load_file_list(day9::Input::data)))
			== 6310675819476);
	}

	SECTION("part 2")
	{
		REQUIRE(
			calc_checksum(
				part_2_compress(
					load_file_list(day9::Example::data1)))
			== 2858);

		REQUIRE(
			calc_checksum(
				part_2_compress(
					load_file_list(day9::Input::data)))
			== 6335972980679);
	}
}
