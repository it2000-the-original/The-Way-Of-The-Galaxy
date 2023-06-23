#   _____ _            _    _               _____  __   _____ _            _____       _                  
#  |_   _| |          | |  | |             |  _  |/ _| |_   _| |          |  __ \     | |                 
#    | | | |__   ___  | |  | | __ _ _   _  | | | | |_    | | | |__   ___  | |  \/ __ _| | __ ___  ___   _ 
#    | | | '_ \ / _ \ | |/\| |/ _` | | | | | | | |  _|   | | | '_ \ / _ \ | | __ / _` | |/ _` \ \/ / | | |
#    | | | | | |  __/ \  /\  / (_| | |_| | \ \_/ / |     | | | | | |  __/ | |_\ \ (_| | | (_| |>  <| |_| |
#    \_/ |_| |_|\___|  \/  \/ \__,_|\__, |  \___/|_|     \_/ |_| |_|\___|  \____/\__,_|_|\__,_/_/\_\\__, |
#                                  __/ |                                                           __/ |
#                                 |___/                                                           |___/ 
#
# This is a simple makefile to compile this project.
# This project is hybrid between visual studio and cmake
# and cross platform, to compile, you need to install the:
# SDL2, SDL2_image, SDL2_ttf, nlohmann_json packages....

cmake_minimum_required (VERSION 3.0.0)

project("The Way Of The Galaxy" VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setting minimal versions of some packages
set(SDL2_MIN_VERSION 2.26.0)
set(JSON_MIN_VERSION 3.2.0)

# Adding packages
find_package(SDL2 ${SDL2_MIN_VERSION} CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(nlohmann_json ${JSON_MIN_VERSION} CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem)

include_directories(
	"${SDL2_INCLUDE_DIRS}"
	"${Boost_INCLUDE_DIRS}"
)

link_directories("${Boost_LIBRARY_DIRS}")

# Setting group files
file(GLOB srcfiles "${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/*.cpp")
file(GLOB components "${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/ECS/*.cpp")

add_executable(TWOTG ${srcfiles} ${components})

target_include_directories(TWOTG PRIVATE 
	"${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/include"
	"${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/include/ECS"
	"${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/include/ECS/custom"
)

target_link_libraries(TWOTG
	${SDL2_LIBRARIES} 
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	nlohmann_json::nlohmann_json
	${Boost_LIBRARIES}
)

# Mentioning necessary files...

set(assetsList "assetsList.json")

set(assets
	"assets/asset2.json"
	"assets/background1_level1.json"
	"assets/laser.json"
	"assets/missile.json"
	"assets/playerSpaceship.json"
)

set(textures
	"assets/textures/backgrounds/Space_Stars2.png"
	"assets/textures/explosions/explosion.png"
	"assets/textures/lasers/laser.png"
	"assets/textures/missiles/missile.png"
	"assets/textures/scraps/pieces.png"
	"assets/textures/spaceships/PolygonConcaveTest.png"
	"assets/textures/spaceships/PolygonConvexTest.png"
	"assets/textures/spaceships/spaceship1.png"
)

set(statusbarTexture "statusbar/statusbar.png")

set(statusbarFonts
	"statusbar/fonts/character.ttf"
	"statusbar/fonts/pixelfonts.ttf"
	"statusbar/fonts/wayfers.ttf"
)

set(statusbarIcons
	"statusbar/icons/energyIcon.png"
	"statusbar/icons/missileIcon.png"
)

set(files_to_copy
	"${assetsList}"
	"${assets}"
	"${textures}"
	"${statusbarTexture}"
	"${statusbarFonts}"
	"${statusbarIcons}"
)

# Copyng necessary files...

foreach(file ${files_to_copy})
	configure_file("${PROJECT_SOURCE_DIR}/The Way Of The Galaxy/${file}" "${PROJECT_BINARY_DIR}/${file}" COPYONLY)
endforeach()