cmake_minimum_required(VERSION 4.2)
project(system_program C)

set(CMAKE_C_STANDARD 11)

add_executable(system_program process_start_exit/daemon.c)
