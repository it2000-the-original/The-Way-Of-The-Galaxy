# This is a simple makefile to compile this project.
# This project is hybrid between visual studio and cmake
# and cross platform, to compile, you need to install the SDL2 SDL2_image SDL2_ttf packages....

cmake_minimum_required (VERSION 3.8)
project ("The Way Of The Galaxy" VERSION 0.0.1)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SDL2_MIN_VERSION 2.26.0)

find_package(SDL2 ${SDL2_MIN_VERSION} REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB srcfiles "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${PROJECT_SOURCE_DIR}/include/*.h")
file(GLOB components "${PROJECT_SOURCE_DIR}/ECS/*.h")

add_executable(TWOTG ${srcfiles})

target_include_directories(TWOTG PRIVATE 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/ECS"
)

target_link_libraries(TWOTG
	${SDL2_LIBRARIES} 
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
)