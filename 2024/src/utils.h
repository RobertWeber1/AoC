#pragma once
#include <ranges>
#include <string_view>
#include <charconv>
#include <algorithm>
#include <point.h>

using namespace std::literals;

#ifdef CALCULATE_RESULTS_AT_COMPILETIME
	#define AOC_REQUIRE STATIC_REQUIRE
#else
	#define AOC_REQUIRE REQUIRE
#endif


inline
constexpr uint64_t string_view_to_uint(std::string_view input)
{
	uint64_t result{};
	std::from_chars(input.data(), input.data()+ input.size(), result);
	return result;
}

inline
constexpr int64_t string_view_to_int(std::string_view input)
{
	int64_t result{};
	std::from_chars(input.data(), input.data()+ input.size(), result);
	return result;
}

enum class Part
{
	one,
	two
};

inline
constexpr auto make_splitter(std::string_view delimiter)
{
	return
		std::ranges::views::split(delimiter)
		| std::ranges::views::filter([](auto && s){ return not s.empty(); })
		| std::ranges::views::transform(
			[](auto && w)
			{
				if(w.empty())
					return std::string_view();

        		return std::string_view(&*w.begin(), std::ranges::distance(w));
			});
}

template<class Func>
constexpr void for_each_line(std::string_view input, Func func)
{
	std::ranges::for_each(input | make_splitter("\n"sv), func);
}


template<class Func>
constexpr void for_each_int(
	std::string_view input,
	std::string_view delimiter,
	Func func)
{
	std::ranges::for_each(
		input
		| make_splitter(delimiter)
		| std::ranges::views::transform(string_view_to_int),
		func);
}


struct MultiStringView
{
	template<size_t N>
	constexpr MultiStringView(std::string_view const (&data)[N])
	: begin_(data)
	, size_(N)
	{}

	constexpr const std::string_view* begin() const
	{
		return begin_;
	}

	constexpr const std::string_view* end() const
	{
		return begin_ + size_;
	}

	constexpr size_t size() const
	{
		return size_;
	}

	constexpr std::string_view operator[](size_t index) const
	{
		return begin_[index];
	}

	constexpr char operator[](Point p) const
	{
		return begin_[p.y][p.x];
	}

	const std::string_view* begin_;
	size_t size_;
};
