#pragma once
#include <string_view>

using namespace std::literals;

namespace day5 {

struct Example
{
	static constexpr std::string_view rules =
R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13
)"sv;

static constexpr std::string_view data =
R"(
75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47
)"sv;

};

} //namespace day5
