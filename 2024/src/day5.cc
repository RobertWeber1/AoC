#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <array>
#include <ranges>

#include <day5/example.h>
#include <day5/input.h>

std::array<std::vector<uint8_t>, 100> parse_ordering_rules(
	std::string_view rules)
{
	std::array<std::vector<uint8_t>, 100> page_rules;

	for_each_line(
		rules,
		[&page_rules](std::string_view line)
		{
			std::ranges::for_each(
				line
				| make_splitter("|"sv)
				| std::ranges::views::transform(string_view_to_int)
				| std::ranges::views::slide(2),
				[&page_rules](auto subrange)
				{
					uint8_t a = *subrange.begin();
					uint8_t b = *(++subrange.begin());
					page_rules[a].push_back(b);
				});
		});

	for(auto & vec : page_rules)
	{
		std::ranges::sort(vec);
	}

	return page_rules;
}

int check_print_order(std::string_view rules, std::string_view data)
{
	auto const page_rules = parse_ordering_rules(rules);

	int sum = 0;
	for_each_line(
		data,
		[&page_rules, &sum](auto line)
		{
			std::vector<uint8_t> page_order;
			for_each_int(line, ","sv,
				[&page_order](int page)
				{
					page_order.push_back(page);
				});

			for(int start = 0; start < page_order.size()-1; ++start)
			{
				uint8_t pivot = page_order[start];
				auto const& page_rule = page_rules[pivot];

				for(int i=start+1; i<page_order.size(); ++i)
				{
					uint8_t current_element = page_order[i];

					if(not std::ranges::binary_search(page_rule, page_order[i]))
					{
						return;
					}
				}
			}
			sum += page_order[page_order.size()/2];
		});

	return sum;
}



int check_fixed_print_order(std::string_view rules, std::string_view data)
{
	auto const page_rules = parse_ordering_rules(rules);

	int sum = 0;
	for_each_line(
		data,
		[&page_rules, &sum](auto line){
			std::vector<uint8_t> page_order;
			for_each_int(line, ","sv,
				[&page_order](int page)
				{
					page_order.push_back(page);
				});


			bool need_fix = false;
			StartOfLoop:
			for(int start = 0; start < page_order.size()-1; ++start)
			{
				uint8_t & pivot = page_order[start];
				auto const& page_rule = page_rules[pivot];

				for(int i=start+1; i<page_order.size(); ++i)
				{
					uint8_t & current_element = page_order[i];
					if(not std::ranges::binary_search(page_rule, current_element))
					{
						std::swap(pivot, current_element);
						need_fix = true;
						goto StartOfLoop;
					}
				}
			}

			if(not need_fix)
			{
				return;
			}

			sum += page_order[page_order.size()/2];
		});

	return sum;
}


TEST_CASE("day 5")
{
	SECTION("part 1")
	{
		REQUIRE(check_print_order(day5::Example::rules, day5::Example::data) == 143);
		REQUIRE(check_print_order(day5::Input::rules, day5::Input::data) == 5509);
	}

	SECTION("part 2")
	{
		REQUIRE(check_fixed_print_order(day5::Example::rules, day5::Example::data) == 123);
		REQUIRE(check_fixed_print_order(day5::Input::rules, day5::Input::data) == 4407);
	}
}
