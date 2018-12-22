cmake_minimum_required(VERSION 3.5)
project(full_processing) # Project Name Here

find_package(wpilib REQUIRED)

set(SRCS_DIR
    src/
    )

set(SRCS
    ${SRCS_DIR}main.cpp
    ${SRCS_DIR}Contour.cpp
    ${SRCS_DIR}GripPipeline.cpp
    )
    
set(HEADERS
    ${SRCS_DIR}Contour.h
    ${SRCS_DIR}GripPipeline.h
    )

add_executable(full_processing.exe ${SRCS} ${HEADERS}) # exectuable name as first parameter
target_link_libraries(full_processing.exe ntcore cscore wpiutil)