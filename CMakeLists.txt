Project (game)
cmake_minimum_required (VERSION 2.6)

Set (PACKAGE "GAME")
Set (VERSION "0.01")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Default definitions
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wunused -Wshadow -Woverloaded-virtual -Wunreachable-code ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-fno-math-errno -fno-exceptions ${CMAKE_CXX_FLAGS}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g0")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -g0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-pg")
  set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "-pg")
endif()

set(BINDIR  "games"             CACHE STRING "Directory from CMAKE_INSTALL_PREFIX where game executable will be installed.")
set(DATADIR "share/games/game"  CACHE STRING "Directory from CMAKE_INSTALL_PREFIX where game data files will be installed.")

If(NOT IS_ABSOLUTE "${BINDIR}")
    set(GAME_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/${BINDIR}/game)
Else(NOT IS_ABSOLUTE "${BINDIR}")
    set(GAME_EXECUTABLE_PATH ${BINDIR}/game)
EndIf(NOT IS_ABSOLUTE "${BINDIR}")

If(NOT IS_ABSOLUTE "${DATADIR}")
    add_definitions(-DDATA_INSTALL_DIR="${CMAKE_INSTALL_PREFIX}/${DATADIR}")
Else(NOT IS_ABSOLUTE "${DATADIR}")
    add_definitions(-DDATA_INSTALL_DIR="${DATADIR}")
EndIf(NOT IS_ABSOLUTE "${DATADIR}")


# Check for SDL2

Find_Package(SDL2)
If (NOT SDL2_FOUND)
    Message (ERROR "Couldn't find SDL2 development files. On Debian-based systems (such as Ubuntu) you should install the 'libsdl2-dev' package.")
Else (NOT SDL2_FOUND)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})
EndIf (NOT SDL2_FOUND)

Find_Package(SDL2_image)
If (NOT SDL2IMAGE_FOUND)
    Message (ERROR "Couldn't find SDL2-image development files. On Debian-based systems (such as Ubuntu) you should install the 'libsdl2-image-dev' package.")
Else (NOT SDL2IMAGE_FOUND)
    INCLUDE_DIRECTORIES(${SDL2IMAGE_INCLUDE_DIR})
EndIf (NOT SDL2IMAGE_FOUND)

Find_Package(SDL2_mixer)
If (NOT SDL2MIXER_FOUND)
    Message (ERROR "Couldn't find SDL2-mixer development files. On Debian-based systems (such as Ubuntu) you should install the 'libsdl2-mixer-dev' package.")
Else (NOT SDL2MIXER_FOUND)
    INCLUDE_DIRECTORIES(${SDL2MIXER_INCLUDE_DIR})
EndIf (NOT SDL2MIXER_FOUND)

Find_Package(SDL2_ttf)
If (NOT SDL2TTF_FOUND)
    Message (ERROR "Couldn't find SDL2-ttf development files. On Debian-based systems (such as Ubuntu) you should install the 'libsdl2-ttf-dev' package.")
Else (NOT SDL2TTF_FOUND)
    INCLUDE_DIRECTORIES(${SDL2TTF_INCLUDE_DIR})
EndIf (NOT SDL2TTF_FOUND)

# Sources

Set (GAME_SOURCES
    ./src/animation.cpp
    ./src/enemy.cpp
    ./src/engines.cpp
    ./src/entity.cpp
    ./src/game_engine.cpp
    ./src/input_engine.cpp
    ./src/map_engine.cpp
    ./src/player.cpp
    ./src/powerup.cpp
    ./src/render_engine.cpp
    ./src/sound_engine.cpp
    ./src/text.cpp
    ./src/utils.cpp
    ./src/main.cpp
)

Set (GAME_HEADERS
    ./src/animation.h
    ./src/common.h
    ./src/enemy.h
    ./src/engines.h
    ./src/entity.h
    ./src/game_engine.h
    ./src/input_engine.h
    ./src/map_engine.h
    ./src/player.h
    ./src/powerup.h
    ./src/render_engine.h
    ./src/sound_engine.h
    ./src/text.h
    ./src/utils.h
)

Add_Executable (game ${GAME_SOURCES} ${GAME_HEADERS})

# libSDLMain comes with libSDL if needed on certain platforms
If (NOT SDL2MAIN_LIBRARY)
  Set (SDL2MAIN_LIBRARY "")
EndIf (NOT SDL2MAIN_LIBRARY)

Target_Link_Libraries (game ${CMAKE_LD_FLAGS} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2MIXER_LIBRARY} ${SDL2TTF_LIBRARY})

# installing to the proper places
install(PROGRAMS
  ${CMAKE_CURRENT_BINARY_DIR}/game
  DESTINATION ${BINDIR})
install(DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/data"
  DESTINATION ${DATADIR})
