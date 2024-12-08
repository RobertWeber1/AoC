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

#include <day7/example.h>
#include <day7/input.h>

std::vector<std::function<uint64_t(uint64_t,uint64_t)>> operators = {
	[](uint64_t a, uint64_t b) -> uint64_t { return a + b; },
	[](uint64_t a, uint64_t b) -> uint64_t { return a * b; }
};

constexpr uint64_t bit_count(uint64_t value)
{
	uint64_t bit_count = 0;
	while(value!=0)
	{
		value>>=1;
		bit_count++;
	}
	return bit_count;
}

std::vector<std::function<uint64_t(uint64_t,uint64_t)>> operators_part2 = {
	[](uint64_t a, uint64_t b) -> uint64_t { return a + b; },
	[](uint64_t a, uint64_t b) -> uint64_t { return a * b; },
	[](uint64_t a, uint64_t b) -> uint64_t {
		const size_t buf_size = 100;
		char buf[buf_size]{};
		std::to_chars_result result = std::to_chars(buf, buf + buf_size, a);
		std::to_chars_result result2 = std::to_chars(result.ptr, buf + buf_size, b);

		if (result.ec != std::errc())
		{
			std::cout << std::make_error_code(result.ec).message() << '\n';
			return 0;
		}

		if (result2.ec != std::errc())
		{
			std::cout << std::make_error_code(result2.ec).message() << '\n';
			return 0;
		}
		uint64_t number = 0;
		std::from_chars(buf, result2.ptr, number);
		return number;
	}
};

uint64_t make_mask(uint64_t val)
{
	return uint64_t(0xFFFFFFFFFFFFFFFFUL)>>(64-val);
}

struct WrappingIndex
{
	WrappingIndex(uint8_t wrap_value, uint8_t value = 0)
	: wrap_value_(wrap_value)
	, value_(value)
	{}

	operator uint8_t() const
	{
		return value_;
	}

	WrappingIndex& operator++()
	{
		if(++value_ == wrap_value_)
		{
			value_ = 0;
		}
		return *this;
	}

	uint8_t get() const
	{
		return value_;
	}

	friend std::ostream& operator<<(std::ostream& out, WrappingIndex const& in)
	{
		return out << (uint16_t)in.get();
	}
private:
	uint8_t wrap_value_;
	uint8_t value_;
};


std::vector<WrappingIndex>& inc_indices(std::vector<WrappingIndex> & in)
{
	for(size_t i = 0; i < in.size(); ++i)
	{
		//uint8_t old = in[i];
		if(++in[i] == 0)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	return in;
}

uint64_t total_calibration_result(
	std::string_view data,
	std::vector<std::function<uint64_t(uint64_t,uint64_t)>> const& ops)
{
	uint64_t sum = 0;

	for_each_line(
		data,
		[&sum, &ops](std::string_view line)
		{
			std::pair<uint64_t, std::vector<uint64_t>> value_list;
			uint64_t index = 0;
			for_each_int(
				line,
				" "sv,
				[&value_list, &index](uint64_t value)
				{
					if(index==0)
					{
						value_list = std::make_pair(value, std::vector<uint64_t>{});
					}
					else
					{
						value_list.second.push_back(value);
					}
					++index;
				});

			std::vector<WrappingIndex> operation_indices(value_list.second.size()-1, WrappingIndex(ops.size(), 0));
			for(uint64_t i=0; i<std::pow(ops.size(), value_list.second.size()-1); ++i)
			{
				uint64_t j = 0;
				auto result = std::ranges::fold_left_first(
					value_list.second,
					[i, &j, &ops, &operation_indices](uint64_t in, uint64_t list_val) -> uint64_t{
						// uint64_t mask = (i >> j) & make_mask(bit_count(ops.size()-1));
						auto const& op_index = operation_indices[j];
						++j;

						return ops[op_index.get()](in, list_val);
					});

				inc_indices(operation_indices);

				if(value_list.first == result)
				{
					sum += value_list.first;
					return;
				}
			}
		});

	return sum;
}

TEST_CASE("concat numbers")
{
	REQUIRE(operators_part2[2](1,0) == 10);
	REQUIRE(operators_part2[2](123,456) == 123456);
	REQUIRE(operators_part2[2](20281182715321,1234) == 202811827153211234);
}

TEST_CASE("make_mask")
{
	// REQUIRE(make_mask(0) == 0b0001);
	REQUIRE(make_mask(1) == 0b0001);
	REQUIRE(make_mask(2) == 0b0011);
	REQUIRE(make_mask(3) == 0b0111);
	REQUIRE(make_mask(4) == 0b1111);
}

TEST_CASE("WrappingIndex")
{
	WrappingIndex i(2);
	REQUIRE(i == 0);
	REQUIRE(++i == 1);
	REQUIRE(++i == 0);
	REQUIRE(++i == 1);
	REQUIRE(++i == 0);

	WrappingIndex i2(3);
	REQUIRE(i2 == 0);
	REQUIRE(++i2 == 1);
	REQUIRE(++i2 == 2);
	REQUIRE(++i2 == 0);
	REQUIRE(++i2 == 1);
	REQUIRE(++i2 == 2);
	REQUIRE(++i2 == 0);
}

TEST_CASE("WrappingIndex vector")
{
	std::vector<WrappingIndex> operation_indices(6, WrappingIndex(3));

	REQUIRE(inc_indices(operation_indices) ==
		std::vector<WrappingIndex>{
			WrappingIndex(3,1),
			WrappingIndex(3,0),
			WrappingIndex(3,0),
			WrappingIndex(3,0),
			WrappingIndex(3,0),
			WrappingIndex(3,0)});

	for(int i=0; i<10; ++i)
	{
		inc_indices(operation_indices);
	}

	REQUIRE(operation_indices ==
		std::vector<WrappingIndex>{
			WrappingIndex(3,2),
			WrappingIndex(3,0),
			WrappingIndex(3,1),
			WrappingIndex(3,0),
			WrappingIndex(3,0),
			WrappingIndex(3,0)});
}


TEST_CASE("day 7")
{
	SECTION("part 1")
	{
		REQUIRE(total_calibration_result(day7::Example::data, operators) == 3749);
		REQUIRE(total_calibration_result(day7::Input::data, operators) == 20281182715321);
	}

	SECTION("part 2")
	{
		REQUIRE(total_calibration_result(day7::Example::data, operators_part2) == 11387);
		REQUIRE(total_calibration_result(day7::Input::data, operators_part2) == 159490400628354);
	}
}
