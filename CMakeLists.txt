cmake_minimum_required(VERSION 3.6)
project(disk_speed)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wc90-c99-compat -Wc99-c11-compat -Wc++-compat -Wextra -Wtype-limits -Wconversion")

set(SOURCE_FILES disk-speed.c)
add_executable(disk_speed ${SOURCE_FILES})