cmake_minimum_required(VERSION 3.5)
project(full_processing) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(full_processing.exe main.cpp) # exectuable name as first parameter
target_link_libraries(full_processing.exe ntcore cscore wpiutil)