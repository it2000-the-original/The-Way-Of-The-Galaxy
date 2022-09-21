cmake_minimum_required (VERSION 3.8)

project ("The Way Of The Galaxy")

file(GLOB srcfiles "*.cpp")
file(GLOB headers "*.h")

add_executable(TWOTG ${srcfiles} ${headers})

target_link_libraries(TWOTG PRIVATE SDL2 SDL2main SDL2_image SDL2_ttf)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Build" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})