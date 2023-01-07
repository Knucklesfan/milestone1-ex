
cmake_minimum_required(VERSION 3.2)

project(supertux)

set(SOURCE_FILES world.cpp type.cpp texture.cpp mousecursor.cpp worldmap.cpp high_scores.cpp particlesystem.cpp lispreader.cpp special.cpp player.cpp menu.cpp text.cpp button.cpp supertux.cpp physic.cpp badguy.cpp setup.cpp leveleditor.cpp level.cpp sound.cpp music_manager.cpp tile.cpp resources.cpp title.cpp sprite_manager.cpp musicref.cpp intro.cpp timer.cpp gameobjs.cpp screen.cpp gameloop.cpp sprite.cpp collision.cpp globals.cpp scene.cpp configfile.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(supertux ${SOURCE_FILES})
target_link_libraries(supertux ${SDL2_LIBRARIES})

#install(TARGETS supertux DESTINATION ${BUILD_DIR})
