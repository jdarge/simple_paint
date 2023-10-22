cmake_minimum_required(VERSION 3.0)
project(SimplePaint)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(simple_paint main.cpp)

target_link_libraries(simple_paint ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

set_target_properties(simple_paint PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

install(TARGETS simple_paint DESTINATION /usr/local/bin)
