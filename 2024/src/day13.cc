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
#include <point.h>
#include <variant>
#include <limits>

#include <day13/example.h>
#include <day13/input.h>

bool cmp(double a, double b)
{
    return (fabs(a - b) <= std::numeric_limits<double>::epsilon() * std::max(fabs(a), fabs(b)));
}

struct ClawMachine
{
	double x1;
	double y1;
	double z1;
	double x2;
	double y2;
	double z2;

	// ClawMachine
	double calc_b() const
	{
		return std::round((z1 - z2 * x1 / x2) / (y1 - y2 * x1 / x2));
	}

	double calc_a(double b) const
	{
		return std::round((z2 - b * y2) / x2);
	}

	bool is_possible(double a, double b) const
	{
		return cmp((a * x1 + b * y1), z1) and cmp((a * x2 + b * y2), z2) and
			(a > 0) /*and (a < 100)*/ and
			(b > 0) /*and (b < 100)*/;
	}

	bool is_possible() const
	{
		auto b = calc_b();
		auto a = calc_a(b);
		return is_possible(a, b);
	}

	uint64_t calc_tokens() const
	{
		auto b = calc_b();
		auto a = calc_a(b);
		if(is_possible(a, b))
		{
			return std::round(a)*3+std::round(b);
		}

		return 0;
	}
};

uint64_t calc_minimal_tokens(MultiStringView input, Part part)
{
	std::vector<ClawMachine> machines;

	for(auto lines_3 : input | std::ranges::views::slide(3) | std::ranges::views::stride(3) )
	{
		auto button_a_str = *lines_3.begin();
		auto button_b_str = *(lines_3.begin()+1);
		auto prize_str = *(lines_3.begin()+2);

		auto [a, x1, x2] = ctre::search<"Button A: X\\+(\\d*), Y\\+(\\d*)">(button_a_str);
		auto [b, y1, y2] = ctre::search<"Button B: X\\+(\\d*), Y\\+(\\d*)">(button_b_str);
		auto [p, z1, z2] = ctre::search<"Prize: X=(\\d*), Y=(\\d*)">(prize_str);

		double correction = (part == Part::one) ? 0 : 10000000000000;
		machines.push_back(ClawMachine{
			double(string_view_to_int(x1.to_view())),
			double(string_view_to_int(y1.to_view())),
			double(string_view_to_int(z1.to_view()))+correction,
			double(string_view_to_int(x2.to_view())),
			double(string_view_to_int(y2.to_view())),
			double(string_view_to_int(z2.to_view()))+correction});
	}

	return std::ranges::fold_left(
		machines,
		uint64_t(0),
		[](uint64_t sum, ClawMachine const& m){ return sum + m.calc_tokens(); });
}

TEST_CASE("ClawMachine")
{
	ClawMachine cm1{94, 22, 8400, 34, 67, 5400};
	REQUIRE(cmp(cm1.calc_b(), 40.0f));
	REQUIRE(cmp(cm1.calc_a(40), 80.0f));
	REQUIRE(cm1.is_possible(80, 40));

	REQUIRE(cm1.calc_tokens() == 280);

	REQUIRE(ClawMachine{26, 67, 12748, 66, 21, 12176}.calc_tokens() == 0);
	REQUIRE(ClawMachine{17, 84, 7870, 86, 37, 6450}.calc_tokens() == 200);
	REQUIRE(ClawMachine{69, 27, 18641, 23, 71, 10279}.calc_tokens() == 0);

	REQUIRE(ClawMachine{26, 67, 10000000012748, 66, 21, 10000000012176}.is_possible());
	REQUIRE(ClawMachine{69, 27, 10000000018641, 23, 71, 10000000010279}.is_possible());
}

TEST_CASE("day 13")
{
	SECTION("part 1")
	{
		REQUIRE(calc_minimal_tokens(day13::Example::data, Part::one) == 480);
		REQUIRE(calc_minimal_tokens(day13::Input::data, Part::one) == 31761);
	}

	SECTION("part 2")
	{
		// REQUIRE(calc_minimal_tokens(day13::Example::data, Part::two) == 480);
		REQUIRE(calc_minimal_tokens(day13::Input::data, Part::two) == 31761);

	}
}

