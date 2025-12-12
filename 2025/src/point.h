#pragma once
#include <fmt/format.h>


struct Point
{
	int x;
	int y;
};

struct Distance
{
	int x;
	int y;
};

struct Vector
{
	Point start;
	Point end;
};


inline
Distance operator-(Point const& lhs, Point const& rhs)
{
	return Distance{rhs.x-lhs.x, rhs.y-lhs.y};
}

inline
Point operator+(Point const& lhs, Distance const& rhs)
{
	return Point{rhs.x+lhs.x, rhs.y+lhs.y};
}

inline
Point operator-(Point const& lhs, Distance const& rhs)
{
	return Point{rhs.x-lhs.x, rhs.y-lhs.y};
}

inline
bool operator==(Point const& lhs, Point const& rhs)
{
	return lhs.x==rhs.x and lhs.y==rhs.y;
}

inline
bool operator<(Point const& lhs, Point const& rhs)
{
	return lhs.x<rhs.x and lhs.y<rhs.y;
}

inline
Distance operator+(Distance const& lhs, Distance const& rhs)
{
	return Distance{rhs.x+lhs.x, rhs.y+lhs.y};
}

template <>
class fmt::formatter<Point>
{
public:
	constexpr auto parse (format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template <typename Context>
	constexpr auto format (Point const& point, Context& ctx) const
	{
		return fmt::format_to(ctx.out(), "Point({}, {})", point.x, point.y);
	}
};

template <>
class fmt::formatter<Distance>
{
public:
	constexpr auto parse (format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template <typename Context>
	constexpr auto format (Distance const& dist, Context& ctx) const
	{
		return fmt::format_to(ctx.out(), "Distance({}, {})", dist.x, dist.y);
	}
};

inline
Point to_point(std::vector<uint64_t> const& input)
{
	if(input.size() != 2)
	{
		throw "Input must contain eactly two values!";
	}

	return Point{static_cast<int>(input[0]), static_cast<int>(input[1])};
}
