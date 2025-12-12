#include <catch2/catch_test_macros.hpp>
#include "utils.h"

struct Machine
{
	uint16_t required_indicator_lights = 0;
	std::vector<uint16_t> button_connections;
	std::vector<uint64_t> joltages;
};

using Machines_t = std::vector<Machine>;

Machines_t get_machine_setup(std::string name)
{
	MemoryMappedFile file(name);

	Machines_t result;

	for_each_line(
		file.get(),
		[&result](std::string_view line)
		{
			Machine machine;
			for_each_line(
				line,
				[&machine](std::string_view entry)
				{
					auto content = entry.substr(1, entry.size()-2);

					if(entry[0] == '[')
					{
						for(int i = 0; i < content.size(); ++i)
						{
							if(content[i] == '#')
							{
								machine.required_indicator_lights |= (1 << i);
							}
						}
					}
					if(entry[0] == '(')
					{
						uint16_t toggles = 0;
						for_each_int(content, ",", [&toggles](uint64_t i){toggles |= (1 << i);});
						machine.button_connections.push_back(toggles);
					}
					if(entry[0] == '{')
					{
						for_each_int(content, ",", [&machine](uint64_t i){machine.joltages.push_back(i);});
					}
				},
				" ");
			result.push_back(machine);
		});

	return result;
}

uint64_t permute_light_switches(Machine const& machine)
{
	uint32_t current_min = std::numeric_limits<uint32_t>::max();
	for(uint32_t i=0; i<std::pow(2, machine.button_connections.size()); ++i)
	{
		uint16_t lights = machine.required_indicator_lights;
		uint32_t current = i;
		uint32_t count = 0;
		uint32_t shift = 0;
		while(current != 0)
		{
			if(current & 1 != 0)
			{
				lights ^= machine.button_connections[shift];
				++count;
			}

			if(lights == 0)
			{
				current_min = std::min(count, current_min);
				break;
			}

			++shift;
			current = (current >> 1);
		}
	}

	return current_min;
}


uint64_t fewest_required_button_presses(Machines_t const& setup)
{
	uint64_t count = 0;
	for(auto const& machine : setup)
	{
		count += permute_light_switches(machine);
	}

	return count;
}

uint64_t permute_button_presses(Machine const& machine)
{
	uint32_t current_min = std::numeric_limits<uint32_t>::max();
	for(uint32_t i=0; i<std::pow(2, machine.button_connections.size()); ++i)
	{
		uint16_t lights = machine.required_indicator_lights;
		uint32_t current = i;
		uint32_t count = 0;
		uint32_t shift = 0;
		while(current != 0)
		{
			if(current & 1 != 0)
			{
				lights ^= machine.button_connections[shift];
				++count;
			}

			if(lights == 0)
			{
				current_min = std::min(count, current_min);
				break;
			}

			++shift;
			current = (current >> 1);
		}
	}

	return current_min;
}

uint64_t fewest_required_button_presses_2(Machines_t const& setup)
{
	uint64_t count = 0;
	for(auto const& machine : setup)
	{
		// fmt::println(
		// 	"indicator: {:b}, buttons: {}, combinations: {}",
		// 	machine.required_indicator_lights,
		// 	machine.button_connections,
		// 	std::pow(2, machine.button_connections.size()));

		count += permute_button_presses(machine);
	}

	return count;
}


TEST_CASE("day 10-1")
{
	REQUIRE(fewest_required_button_presses(get_machine_setup("./input/day10/example.txt")) == 7);
	REQUIRE(fewest_required_button_presses(get_machine_setup("./input/day10/input.txt")) == 466);
}

TEST_CASE("day 10-2")
{
	REQUIRE(fewest_required_button_presses_2(get_machine_setup("./input/day10/example.txt")) == 33);
	// REQUIRE(fewest_required_button_presses(get_machine_setup("./input/day10/input.txt")) == 466);
}
