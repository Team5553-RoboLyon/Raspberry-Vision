cmake_minimum_required(VERSION 3.5)
project(full_processing) # Project Name Here

find_package(wpilib REQUIRED)

set(SRCS
    main.cpp
    pipeline/Contour.cpp
    pipeline/GripPipeline.cpp
    )
    
set(HEADERS
    pipeline/Contour.h
    pipeline/GripPipeline.h
    )

add_executable(full_processing.exe ${SRCS} ${HEADERS}) # exectuable name as first parameter
target_link_libraries(full_processing.exe ntcore cscore wpiutil)