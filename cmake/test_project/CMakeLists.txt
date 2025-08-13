# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/daw_thunk
#

cmake_minimum_required(VERSION 3.24)

project("install-test"
  VERSION "1.0.0"
  DESCRIPTION "Tests that find_package works"
  HOMEPAGE_URL "https://github.com/beached/daw_thunk"
  LANGUAGES C CXX
)

add_executable(${PROJECT_NAME} test.cpp)
target_compile_options(${PROJECT_NAME} INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/permissive->)
target_link_libraries(${PROJECT_NAME} PRIVATE daw::daw-thunk)

enable_testing()
add_test(${PROJECT_NAME}_test ${PROJECT_NAME})
