# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/main.c)
zephyr_include_directories(src)
# zephyr_ld_options(-Wl,-u,B_func)
zephyr_link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/src/moduleA.a)
zephyr_link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/src/moduleB.a)
zephyr_link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/src/moduleC.a)
