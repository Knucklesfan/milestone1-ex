
cmake_minimum_required(VERSION 3.2)

project(supertux)
add_compile_definitions(DATA_PREFIX="../data")
add_compile_definitions(VERSION="0.1.5SDL2")
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(SOURCE_FILES world.cpp type.cpp texture.cpp mousecursor.cpp worldmap.cpp high_scores.cpp particlesystem.cpp lispreader.cpp special.cpp player.cpp menu.cpp text.cpp button.cpp supertux.cpp physic.cpp badguy.cpp setup.cpp leveleditor.cpp level.cpp sound.cpp music_manager.cpp tile.cpp resources.cpp title.cpp sprite_manager.cpp musicref.cpp intro.cpp timer.cpp gameobjs.cpp screen.cpp gameloop.cpp sprite.cpp collision.cpp globals.cpp scene.cpp configfile.cpp background.cpp)
#ironically, borrowed exactly from milestone 2 of supertux lol
message(STATUS "Checking for SDL2")
if(WIN32)
  if(VCPKG_BUILD)
    find_package(SDL2 CONFIG REQUIRED)
    find_library(SDL2IMAGE_LIBRARIES SDL2_image)
    find_library(SDL2MAIN_LIBRARIES SDL2main)
  else()
    find_path(SDL2_INCLUDE_DIRS NAMES SDL.h PATHS "${DEPENDENCY_FOLDER}/include/SDL2")
    find_path(SDL2IMAGE_INCLUDE_DIRS NAMES SDL_image.h PATHS "${DEPENDENCY_FOLDER}/include/SDL2_image")
    find_library(SDL2_LIBRARIES NAMES SDL2 PATHS "${DEPENDENCY_FOLDER}/lib")
    find_library(SDL2IMAGE_LIBRARIES NAMES SDL2_image PATHS "${DEPENDENCY_FOLDER}/lib")
    find_library(SDL2MAIN_LIBRARIES NAMES SDL2main PATHS "${DEPENDENCY_FOLDER}/lib")
  endif()
else()
  if(NOT EMSCRIPTEN)
    if(VCPKG_BUILD)
      find_package(SDL2 CONFIG REQUIRED)
      find_package(sdl2-image CONFIG REQUIRED)
      find_package(sdl2-mixer CONFIG REQUIRED)
      find_path(SDL2_INCLUDE_DIRS SDL2/SDL.h)
      find_path(SDL2IMAGE_INCLUDE_DIRS SDL2/SDL_image.h)
      find_path(SDL2MIXER_INCLUDE_DIRS SDL2/SDL_mixer.h)

      set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS}/SDL2)
      set(SDL2IMAGE_INCLUDE_DIRS ${SDL2IMAGE_INCLUDE_DIRS}/SDL2)
      set(SDL2MIXER_INCLUDE_DIRS ${SDL2MIXER_INCLUDE_DIRS}/SDL2)

      find_library(SDL2_LIBRARIES SDL2)
      find_library(SDL2IMAGE_LIBRARIES SDL2_image)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
      # 32-bit
      find_library(SDL2_LIBRARIES SDL2)
      find_library(SDL2IMAGE_LIBRARIES SDL2_image)
      find_library(SDL2MIXER_LIBRARIES SDL2_mixer)

      find_path(SDL2_INCLUDE_DIRS SDL2/SDL.h)
      find_path(SDL2IMAGE_INCLUDE_DIRS SDL2/SDL_image.h)
      find_path(SDL2MIXER_INCLUDE_DIRS SDL2/SDL_mixer.h)

      set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS}/SDL2)
      set(SDL2IMAGE_INCLUDE_DIRS ${SDL2IMAGE_INCLUDE_DIRS}/SDL2)
      set(SDL2MIXER_INCLUDE_DIRS ${SDL2MIXER_INCLUDE_DIRS}/SDL2)

    else()
      # 64-bit
      include(FindPkgConfig)
      if(NOT PKG_CONFIG_FOUND)
        message(WARNING "Trying to find libraries using pkg_search_module, but pkg-config was not found. Install pkg-config to continue.")
      else()
        pkg_search_module(SDL2 REQUIRED sdl2>=2.0.1)
        pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
        pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

      endif()
    endif()
  endif()
endif()
set(HAVE_SDL TRUE)

message(STATUS "  SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(STATUS "  SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "  SDL2IMAGE_LIBRARIES: ${SDL2IMAGE_LIBRARIES}")
message(STATUS "  SDL2IMAGE_INCLUDE_DIRS: ${SDL2IMAGE_INCLUDE_DIRS}")
message(STATUS "  SDL2MIXER_LIBRARIES: ${SDL2MIXER_LIBRARIES}")
message(STATUS "  SDL2MIXER_INCLUDE_DIRS: ${SDL2MIXER_INCLUDE_DIRS}")

add_library(LibSDL2 INTERFACE IMPORTED)
set_target_properties(LibSDL2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
  INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARIES}")

add_library(LibSDL2main INTERFACE IMPORTED)
set_target_properties(LibSDL2main PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
  INTERFACE_LINK_LIBRARIES "${SDL2MAIN_LIBRARIES}")

add_library(LibSDL2_image INTERFACE IMPORTED)
set_target_properties(LibSDL2_image PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${SDL2IMAGE_INCLUDE_DIRS}"
  INTERFACE_LINK_LIBRARIES "${SDL2IMAGE_LIBRARIES}")

mark_as_advanced(
  SDL2_INCLUDE_DIRS
  SDL2_LIBRARIES
  )

mark_as_advanced(
  SDL2IMAGE_INCLUDE_DIRS
  SDL2IMAGE_LIBRARIES
  )

find_package(ZLIB)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include_directories(supertux ${SDL2_INCLUDE_DIRS})
add_executable(supertux ${SOURCE_FILES})
target_link_libraries(supertux z)
target_link_libraries(supertux GL)
target_link_libraries(supertux GLEW)

target_link_libraries(supertux SDL2)
target_link_libraries(supertux SDL2_mixer)
target_link_libraries(supertux SDL2_image)



#install(TARGETS supertux DESTINATION ${BUILD_DIR})
