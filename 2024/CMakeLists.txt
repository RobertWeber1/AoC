cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  ctre
  GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
  GIT_TAG        main)
FetchContent_MakeAvailable(ctre)

#------------------------------------------------------------------------------#

project(AoC2024)

add_executable(${PROJECT_NAME}
	src/day1.cc
	src/day2.cc
	src/day3.cc
	src/day4.cc
	src/day5.cc
	src/day6.cc
	src/day7.cc
	src/day8.cc
	src/day9.cc
	src/day10.cc
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE asan
	PRIVATE ubsan
	PRIVATE Catch2::Catch2WithMain
	PRIVATE fmt::fmt
	PRIVATE ctre::ctre
)

target_include_directories(${PROJECT_NAME}
	PRIVATE src
	PRIVATE input
)

# target_compile_definitions(${PROJECT_NAME}
# 	PRIVATE -DCALCULATE_RESULTS_AT_COMPILETIME=1
# )

target_compile_options(${PROJECT_NAME}
	# PRIVATE -fsanitize=undefined
	# PRIVATE -fsanitize=address
	PRIVATE -fconstexpr-ops-limit=4290000000
)

# target_link_options(${PROJECT_NAME}
	# PRIVATE -fsanitize=undefined
	# PRIVATE -fsanitize=address
# )

target_compile_features(${PROJECT_NAME}
	PRIVATE cxx_std_23
)
