cmake_minimum_required(VERSION 3.5)
project(hsl_threshold) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(hsl_threshold.exe main.cpp) # exectuable name as first parameter
target_link_libraries(hsl_threshold.exe ntcore cscore wpiutil)