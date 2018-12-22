cmake_minimum_required(VERSION 3.5)
project(acquire_and_stream_video) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(acquire_and_stream_video.exe main.cpp) # exectuable name as first parameter
target_link_libraries(acquire_and_stream_video.exe ntcore cscore wpiutil)