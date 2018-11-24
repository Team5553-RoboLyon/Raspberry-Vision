cmake_minimum_required(VERSION 3.5)
project(nt_client) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(nt_client.exe main.cpp) # exectuable name as first parameter
target_link_libraries(nt_client.exe ntcore cscore wpiutil)