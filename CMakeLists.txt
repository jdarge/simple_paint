cmake_minimum_required(VERSION 3.25)

# Setting a variable to use for both the project name and the resulting binary executable
set(REPO simple_paint)
project(${REPO})
set(PROJECT_NAME ${REPO})

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Compiler flags (for C++)
set(CPP_FLAGS "-Wall -Wextra -pedantic")

# Add the src directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add all the source files in the src directory to the project
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp) # Change the file extension to .cpp for C++

# Create the executable from the source files
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/driver.cpp ${SOURCES}) # Change the file extension to .cpp for C++

# Set the output directory to the build/bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    COMPILE_FLAGS ${CPP_FLAGS}
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin
)

# Set the object directory to the build/obj directory
set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY
    CMAKE_OBJECT_PATH_PREFIX ${PROJECT_SOURCE_DIR}/build/obj/
)

# Set the cmake binary directory to the build/cmake directory
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build/cmake)

# Add the build/cmake directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/build/cmake)

# Add any other dependencies here, such as libraries or additional build options
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(simple_paint ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

