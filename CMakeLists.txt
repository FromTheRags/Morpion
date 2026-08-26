cmake_minimum_required(VERSION 3.16)
project(morpion LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL 1.2
find_package(SDL REQUIRED)

# Find SDL_ttf
find_path(SDL_TTF_INCLUDE_DIR SDL_ttf.h
    PATH_SUFFIXES SDL include/SDL include
    PATHS
        ${SDL_INCLUDE_DIR}
        /usr/include/SDL
        /usr/local/include/SDL
        /opt/homebrew/include/SDL
        /opt/local/include/SDL
)

find_library(SDL_TTF_LIBRARY
    NAMES SDL_ttf
    PATH_SUFFIXES lib
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/homebrew/lib
        /opt/local/lib
)

if(NOT SDL_TTF_INCLUDE_DIR OR NOT SDL_TTF_LIBRARY)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKG_SDL_TTF QUIET SDL_ttf)
        if(PKG_SDL_TTF_FOUND)
            set(SDL_TTF_INCLUDE_DIR ${PKG_SDL_TTF_INCLUDE_DIRS})
            set(SDL_TTF_LIBRARY ${PKG_SDL_TTF_LIBRARIES})
        endif()
    endif()
endif()

if(NOT SDL_TTF_LIBRARY)
    message(FATAL_ERROR "SDL_ttf library not found. Please install libsdl-ttf2.0-dev or equivalent.")
endif()

# Source files
set(SOURCES main.cpp)

# Windows icon resource
if(WIN32)
    if(MINGW OR MSVC)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icone.rc")
            list(APPEND SOURCES icone.rc)
        endif()
    endif()
endif()

# Executable target
add_executable(morpion ${SOURCES})

target_include_directories(morpion PRIVATE
    ${SDL_INCLUDE_DIR}
    ${SDL_TTF_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(morpion PRIVATE
    ${SDL_LIBRARY}
    ${SDL_TTF_LIBRARY}
)

if(MINGW)
    target_link_libraries(morpion PRIVATE mingw32 SDLmain)
    target_link_options(morpion PRIVATE -mwindows)
endif()

# Assets to copy to the output directory
set(GAME_ASSETS
    grille.bmp
    nice.bmp
    croix.bmp
    rond.bmp
    sdl_icone.bmp
    PatrickHand-Regular.ttf
    Graduate-Regular.ttf
)

foreach(ASSET ${GAME_ASSETS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}")
        add_custom_command(
            TARGET morpion POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
                "$<TARGET_FILE_DIR:morpion>/${ASSET}"
            COMMENT "Copying ${ASSET} to output directory"
        )
    endif()
endforeach()

# Install rules
install(TARGETS morpion DESTINATION bin)
install(FILES ${GAME_ASSETS} DESTINATION bin)
