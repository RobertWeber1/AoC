#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>
#include <array>


const MemoryMappedFile example(FileDescriptor("./input/day4/example.txt"));
const MemoryMappedFile input(FileDescriptor("./input/day4/input.txt"));

struct Dimensions
{
	int height;
	int width;
};

constexpr Dimensions get_shape(std::string_view data)
{
	Dimensions result = {0, 0};
	for(int i = 0; i<data.size(); ++i)
	{
		if(data[i] == '\n')
		{
			result.width = i;
			result.height = data.size() / (result.width+1);
			break;
		}
	}

	for(int j = result.width; j<data.size(); j+=(result.width+1))
	{
	 	if(data[j] != '\n')
		{
			throw std::logic_error("input is misshaped!");
		}
	}

	return result;
}

using region_t = std::array<std::string_view, 7>;

constexpr std::string_view get_sub_row_around(int x, int y, std::string_view data, Dimensions dimensions, int width)
{
	int pos = (x-width/2) + y*(dimensions.width+1);

	return std::string_view(data.data()+pos, width);
};

constexpr char get_char(int x, int y, std::string_view data, Dimensions dimensions)
{
	return data[x + y*(dimensions.width+1)];
};


using region2_t = std::array<std::string_view, 3>;

std::array<region2_t, 4> masks2 = {
		region2_t{
			"M  "sv,
			" A "sv,
			"  S"sv},
		region2_t{
			"  S"sv,
			" A "sv,
			"M  "sv},
		region2_t{
			"S  "sv,
			" A "sv,
			"  M"sv},
		region2_t{
			"  M"sv,
			" A "sv,
			"S  "sv}};

void print_region_2_around(
	int x,
	int y,
	std::string_view data,
	Dimensions dimensions,
	int matches = 0)
{
	const region_t in_region ={
		get_sub_row_around(x, y-1, data, dimensions, 3),
		get_sub_row_around(x, y,   data, dimensions, 3),
		get_sub_row_around(x, y+1, data, dimensions, 3)};

	fmt::println("-----");
	fmt::println("|{}|", in_region[0]);
	fmt::println("|{}|", in_region[1]);
	fmt::println("|{}|", in_region[2]);
	fmt::println("--{}--", matches);
}

constexpr int check_region_2(region2_t const& in_region, region2_t const& mask)
{
	int count = 0;
	for(int i = 0; i<in_region.size(); ++i)
	{
		for(auto const [value, mask] : std::ranges::views::zip(in_region[i], mask[i]))
		{
			count += (((mask!=' ') and (mask==value)) ? 1 : 0);
		}
	}

	return (count == 3) ? 1 : 0;
}

constexpr int check_region_2_around(int x, int y, std::string_view data, Dimensions dimensions)
{
	const region2_t in_region ={
		get_sub_row_around(x, y-1, data, dimensions, 3),
		get_sub_row_around(x, y,   data, dimensions, 3),
		get_sub_row_around(x, y+1, data, dimensions, 3)};

	if(get_char(x, y, data, dimensions) != 'A')
	{
		return 0;
	}

	int count = 0;
	for(auto const& mask : masks2)
	{
		count += check_region_2(in_region, mask);
	}

	count = (count == 2) ? 1 : 0;

	return count;
}


constexpr int count_x_mas(std::string_view data)
{

	auto const dimensions = get_shape(data);

	int count = 0;
	for(int y = 3; y<=dimensions.height-4; ++y)
	{
		for(int x = 3; x<=dimensions.width-4; ++x)
		{
			count += check_region_2_around(x, y, data, dimensions);
		}
	}

	return count;
}
//----------------------------------------------------------------------------//
std::array<region_t, 8> masks = {
		region_t{
			"S      "sv,
			" A     "sv,
			"  M    "sv,
			"   X   "sv,
			"       "sv,
			"       "sv,
			"       "sv},
		region_t{
			"   S   "sv,
			"   A   "sv,
			"   M   "sv,
			"   X   "sv,
			"       "sv,
			"       "sv,
			"       "sv},
		region_t{
			"      S"sv,
			"     A "sv,
			"    M  "sv,
			"   X   "sv,
			"       "sv,
			"       "sv,
			"       "sv},
		region_t{
			"       "sv,
			"       "sv,
			"       "sv,
			"   XMAS"sv,
			"       "sv,
			"       "sv,
			"       "sv},
		region_t{
			"       "sv,
			"       "sv,
			"       "sv,
			"   X   "sv,
			"    M  "sv,
			"     A "sv,
			"      S"sv},
		region_t{
			"       "sv,
			"       "sv,
			"       "sv,
			"   X   "sv,
			"   M   "sv,
			"   A   "sv,
			"   S   "sv},
		region_t{
			"       "sv,
			"       "sv,
			"       "sv,
			"   X   "sv,
			"  M    "sv,
			" A     "sv,
			"S      "sv},
		region_t{
			"       "sv,
			"       "sv,
			"       "sv,
			"SAMX   "sv,
			"       "sv,
			"       "sv,
			"       "sv}};

void print_region_around(
	int x,
	int y,
	std::string_view data,
	Dimensions dimensions,
	int matches = 0)
{
	const region_t in_region ={
		get_sub_row_around(x, y-3, data, dimensions, 7),
		get_sub_row_around(x, y-2, data, dimensions, 7),
		get_sub_row_around(x, y-1, data, dimensions, 7),
		get_sub_row_around(x, y,   data, dimensions, 7),
		get_sub_row_around(x, y+1, data, dimensions, 7),
		get_sub_row_around(x, y+2, data, dimensions, 7),
		get_sub_row_around(x, y+3, data, dimensions, 7)};

	fmt::println("---------");
	fmt::println("|{}|", in_region[0]);
	fmt::println("|{}|", in_region[1]);
	fmt::println("|{}|", in_region[2]);
	fmt::println("|{}|", in_region[3]);
	fmt::println("|{}|", in_region[4]);
	fmt::println("|{}|", in_region[5]);
	fmt::println("|{}|", in_region[6]);
	fmt::println("----{}----", matches);
}

constexpr int check_region(region_t const& in_region, region_t const& mask)
{
	int count = 0;
	for(int i = 0; i<in_region.size(); ++i)
	{
		for(auto const [value, mask] : std::ranges::views::zip(in_region[i], mask[i]))
		{
			count += (((mask!=' ') and (mask==value)) ? 1 : 0);
		}
	}
	return (count == 4) ? 1 : 0;
}


constexpr int check_region_around(int x, int y, std::string_view data, Dimensions dimensions)
{
	const region_t in_region ={
		get_sub_row_around(x, y-3, data, dimensions, 7),
		get_sub_row_around(x, y-2, data, dimensions, 7),
		get_sub_row_around(x, y-1, data, dimensions, 7),
		get_sub_row_around(x, y,   data, dimensions, 7),
		get_sub_row_around(x, y+1, data, dimensions, 7),
		get_sub_row_around(x, y+2, data, dimensions, 7),
		get_sub_row_around(x, y+3, data, dimensions, 7)};

	if(get_char(x, y, data, dimensions) != 'X')
	{
		return 0;
	}

	int count = 0;
	for(auto const& mask : masks)
	{
		count += check_region(in_region, mask);
	}

	return count;
}


constexpr int count_xmas(std::string_view data)
{
	auto const dimensions = get_shape(data);

	int count = 0;
	for(int y = 3; y<=dimensions.height-4; ++y)
	{
		for(int x = 3; x<=dimensions.width-4; ++x)
		{
			count += check_region_around(x, y, data, dimensions);
		}
	}

	return count;
}


TEST_CASE("day 4")
{
	REQUIRE(count_xmas(example.get()) == 18);
	REQUIRE(count_xmas(input.get()) == 2547);

	REQUIRE(count_x_mas(example.get()) == 9);
	REQUIRE(count_x_mas(input.get()) == 1939);
}
