#include <catch2/catch_test_macros.hpp>
#include "utils.h"

struct Machine
{
	uint16_t required_indicator_lights;
	std::vector<uint16_t> button_connections;
	std::vector<int> joltages;
};

using Machines_t = std::vector<Machine>;

Machines_t get_machine_setup(std::string name)
{
	Machines_t result;

	return result;
}

uint64_t fewest_required_button_presses(Machines_t const& setup)
{
	return 0;
}


TEST_CASE("day 10-1")
{
	REQUIRE(fewest_required_button_presses(get_machine_setup("./input/day10/example.txt")) == 7);
}
