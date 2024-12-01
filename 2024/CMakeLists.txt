cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

project(AoC2024)

add_executable(${PROJECT_NAME}
	src/day1.cc
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE asan
	PRIVATE ubsan
	PRIVATE Catch2::Catch2WithMain
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
