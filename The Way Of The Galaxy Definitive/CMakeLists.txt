cmake_minimum_required (VERSION 3.8)

project ("The Way Of The Galaxy")

file(GLOB srcfiles "*.cpp")
file(GLOB headers "include/*.h")
file(GLOB components "ECS/*.h")

add_executable(TWOTG ${srcfiles} ${headers} ${components})

target_include_directories(TWOTG PRIVATE "include" "ECS")

target_link_libraries(TWOTG PRIVATE SDL2 SDL2main SDL2_image SDL2_ttf)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TWOTG PROPERTY CXX_STANDARD 20)
endif()