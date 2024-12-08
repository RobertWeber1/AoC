#include <catch2/catch_test_macros.hpp>
#include <read_file.h>
#include <utils.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ctre.hpp>
#include <iostream>
#include <array>

#include <day6/example.h>
#include <day6/input.h>

struct Pose
{
	struct Position
	{
		uint16_t x;
		uint16_t y;
	}position;

	enum Direction : char
	{
		North = '^',
		East  = '>',
		South = 'v',
		West  = '<'
	}direction;

	static Position advance(Position pos, Direction dir)
	{
		switch(dir)
		{
		case North: --pos.y; break;
		case East:  ++pos.x; break;
		case South: ++pos.y; break;
		case West:  --pos.x; break;
		}

		return pos;
	}

	static Pose advance(Pose current)
	{
		current.position = advance(current.position, current.direction);
		return current;
	}

	Pose advance()
	{
		return advance(*this);
	}

	constexpr Direction turn_right(Direction in)
	{
		switch(in)
		{
		case North: return East;
		case East:  return South;
		case South: return West;
		case West:  return North;
		}

		throw std::logic_error("Invalid guard direction!");
	};

	Pose& turn_right()
	{
		direction = turn_right(direction);
		return *this;
	}

	bool operator==(Pose const& other) const
	{
		return
			position.x == other.position.x and
			position.y == other.position.y and
			direction == other.direction;
	}

};

bool operator==(Pose::Position const& lhs, Pose::Position const& rhs)
{
	return lhs.x == rhs.x and lhs.y == rhs.y;
}

struct Guard
{
	Pose pose;
	std::vector<std::vector<char>> floor;
	bool done = false;
	int counter = 1;
	bool looping = false;

	static Guard find_initial(auto const& data)
	{
		std::vector<std::vector<char>> input;
		for(auto line : data)
		{
			input.push_back(std::vector<char>(line.begin(), line.end()));
		}

		for(uint16_t y=0; y<(sizeof(data)/sizeof(std::string_view)); ++y)
		{
			std::string_view line = data[y];
			for(uint16_t x=0; x<line.size(); ++x)
			{
				if(line[x] != '.' and line[x] != '#')
				{
					// input[6][3] = '#';
					return Guard{
						Pose{{x, y},
						static_cast<Pose::Direction>(line[x])},
						std::move(input),
						false};
				}
			}
		}

		throw std::logic_error("Guard position not found!");
	}

	bool next()
	{
		for(int i=0; i<2; ++i)
		{
			const auto next_pose = pose.advance();
			if(next_pose.position.y >= floor.size())
			{
				done = true;
				return false;
			}

			if(next_pose.position.x >= floor[next_pose.position.y].size())
			{
				done = true;
				return false;
			}

			if(floor[next_pose.position.y][next_pose.position.x]=='#')
			{
				pose.turn_right();
				continue;
			}
			else
			{
				if(floor[next_pose.position.y][next_pose.position.x]=='.')
				{
					++counter;
					floor[next_pose.position.y][next_pose.position.x] =
						(char)pose.direction;
				}
				else
				{
					if(floor[next_pose.position.y][next_pose.position.x] ==
					   (char)pose.direction)
					{
						looping = true;
						return false;
					}
				}

				pose = next_pose;
				break;
			}
		}

		return true;
	}
};

void print_floor(Guard const& guard)
{
	for(auto const& row : guard.floor)
	{
		for(char c : row)
		{
			fmt::print("{}", c);
		}
		puts("");
	}
	puts("\n");
}

int count_guarded_positions(auto const& data)
{
	Guard guard = Guard::find_initial(data);

	while(guard.next())
	{}

	return guard.counter;
}

int count_guarded_loops(auto const& data)
{
	Guard guard = Guard::find_initial(data);
	Pose initial_pose = guard.pose;
	while(guard.next())
	{}

	int loops = 0;
	for(uint16_t y=0; y<guard.floor.size(); ++y)
	{
		for(uint16_t x=0; x<guard.floor[y].size(); ++x)
		{
			char c = guard.floor[y][x];
			if(c=='.' or c=='#')
			{
				continue;
			}

			if(initial_pose.position.x == x and initial_pose.position.y == y)
			{
				continue;
			}

			Guard fooled_guard = Guard::find_initial(data);
			fooled_guard.floor[y][x] = '#';
			while(fooled_guard.next())
			{}
			if(fooled_guard.looping)
			{
				++loops;
			}
		}
	}

	return loops;
}

TEST_CASE("advance position")
{
	REQUIRE(
		Pose::advance(Pose::Position{5,5}, Pose::Direction::North) ==
		Pose::Position{5,4});

	REQUIRE(
		Pose::advance(Pose::Position{5,5}, Pose::Direction::East) ==
		Pose::Position{6,5});

	REQUIRE(
		Pose::advance(Pose::Position{5,5}, Pose::Direction::South) ==
		Pose::Position{5,6});

	REQUIRE(
		Pose::advance(Pose::Position{5,5}, Pose::Direction::West) ==
		Pose::Position{4,5});
}

TEST_CASE("day 6")
{
	SECTION("part 1")
	{
		REQUIRE(count_guarded_positions(day6::Example::data) == 41);
		REQUIRE(count_guarded_positions(day6::Input::data) == 5305);
	}

	SECTION("part 2")
	{
		REQUIRE(count_guarded_loops(day6::Example::data) == 6);
		REQUIRE(count_guarded_loops(day6::Input::data) == 2143);
	}
}
