cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  ctre
  GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
  GIT_TAG        main)
FetchContent_MakeAvailable(ctre)


project(AoC2024)

add_executable(${PROJECT_NAME}
	src/day1.cc
	src/day2.cc
	src/day3.cc
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
)

target_compile_options(${PROJECT_NAME}
	PRIVATE -fsanitize=undefined
	PRIVATE -fsanitize=address
)

target_link_options(${PROJECT_NAME}
	PRIVATE -fsanitize=undefined
	PRIVATE -fsanitize=address
)

target_compile_features(${PROJECT_NAME}
	PRIVATE cxx_std_23
)


# target_compile_options(${PROJECT_NAME}-test
# 	PRIVATE -fcoroutines
# )
