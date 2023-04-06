cmake_minimum_required(VERSION 3.22)
SET(COVERAGE OFF CACHE BOOL "Coverage")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(WorkWithGtest)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest"
	"gtest"
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/banking
)

add_executable(
	testBanking
	tests/testBanking.cpp
)

if (COVERAGE)
    target_compile_options(testBanking PRIVATE --coverage)
    target_link_libraries(testBanking PRIVATE --coverage)
endif()

target_include_directories(
	testBanking PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/banking"
)


target_link_libraries(
	testBanking PRIVATE
	gtest gtest_main banking gmock_main
)

