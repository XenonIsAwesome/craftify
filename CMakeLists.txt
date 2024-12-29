cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CraftTV)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add the executable
file(GLOB SOURCES "src/*.cpp")
add_executable(craft_tv_convert_frame ${SOURCES} src/nlohmann/json.hpp)

# Link OpenCV libraries
target_link_libraries(craft_tv_convert_frame ${OpenCV_LIBS})

# Set C++ standard
set_target_properties(craft_tv_convert_frame PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)