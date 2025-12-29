cmake_minimum_required(VERSION 3.5)
project(Mental)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
	lexy
	URL https://lexy.foonathan.net/download/lexy-src.zip
)

FetchContent_MakeAvailable(lexy)

add_executable(mental)
target_sources(mental PRIVATE main.cpp)
target_compile_options(mental PRIVATE -O3)
target_link_libraries(mental PRIVATE foonathan::lexy)