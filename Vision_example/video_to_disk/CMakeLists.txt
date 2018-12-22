cmake_minimum_required(VERSION 3.5)
project(video_to_disk) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(video_to_disk.exe main.cpp) # exectuable name as first parameter
target_link_libraries(video_to_disk.exe ntcore cscore wpiutil)