cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CraftifyCPP)

# Add an option for STEP_DEBUG
option(STEP_DEBUG "Enable step debug mode" OFF)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(craftify_convert_frame src/nlohmann/single_include/nlohmann/json.hpp
                                      src/convertors/convert_frame.cpp
                                      src/convertors/convert_frame.h
                                      src/objects/baked_block.cpp
                                      src/objects/baked_block.h
                                      src/objects/blocks_db.cpp
                                      src/objects/blocks_db.h
                                      src/main.cpp)

# Link OpenCV libraries
target_link_libraries(craftify_convert_frame ${OpenCV_LIBS})

# Set C++ standard
set_target_properties(craftify_convert_frame PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Enable debug properties if STEP_DEBUG is ON
if(STEP_DEBUG)
    target_compile_definitions(craftify_convert_frame PRIVATE STEP_DEBUG)
    message(STATUS "STEP_DEBUG is enabled")
endif()