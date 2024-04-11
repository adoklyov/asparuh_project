cmake_minimum_required(VERSION 3.0.0)
project(WarWithCards VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

include(CTest)
enable_testing()

include_directories(/usr/include/SDL2)

add_executable(WarWithCards main.cpp App.cpp Deck.cpp Player.cpp Button.cpp)

add_library(pugixml /home/default/Downloads/pugixml-1.14/src/pugixml.cpp)

target_link_libraries(WarWithCards SDL2 SDL2_image SDL2_mixer SDL2_ttf pugixml)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
