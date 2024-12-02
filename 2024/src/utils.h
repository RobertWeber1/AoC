#pragma once
#include <ranges>
#include <string_view>
#include <charconv>
#include <algorithm>

using namespace std::literals;

inline
int string_view_to_int(std::string_view input)
{
	int result{};
	std::from_chars(input.data(), input.data()+ input.size(), result);
	return result;
}

enum class Part
{
	one,
	two
};


auto make_splitter(std::string_view && delimiter)
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
void for_each_line(std::string_view input, Func func)
{
	std::ranges::for_each(input | make_splitter("\n"sv), func);
}


template<class Func>
void for_each_int(std::string_view input, Func func)
{
	std::ranges::for_each(
		input
		| make_splitter(" "sv)
		| std::ranges::views::transform(string_view_to_int),
		func);
}
