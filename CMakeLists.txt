cmake_minimum_required(VERSION 3.10)
project(i8080-spaceinvaders-emulator C)

set(CMAKE_C_STANDARD 11)

# Debug option for CPU tracing
option(DEBUG_CPU "Enable CPU instruction tracing and register dumps" OFF)

# Add SDL2 from submodule
set(SDL_SHARED OFF CACHE BOOL "Build SDL2 as static library")
set(SDL_STATIC ON CACHE BOOL "Build SDL2 as static library")
add_subdirectory(external/SDL2)

# Source files organized by layer
set(EMULATOR_SOURCES
    src/emulator/i8080.c
    src/emulator/opcodes.c
    src/emulator/memory.c
    src/emulator/disassembler.c
)

set(MACHINE_SOURCES
    src/machine/emulator.c
    src/machine/io.c
    src/machine/hexdump.c
)

set(PLATFORM_SOURCES
    src/platform/platform.c
    src/platform/display.c
    src/platform/input.c
)

# Executable
add_executable(spaceinvaders
    src/main.c
    ${EMULATOR_SOURCES}
    ${MACHINE_SOURCES}
    ${PLATFORM_SOURCES}
)

# Include directories
target_include_directories(spaceinvaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/emulator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/machine
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/rsrc
)

# Link SDL2
target_link_libraries(spaceinvaders PRIVATE SDL2::SDL2-static SDL2::SDL2main)

# Add debug flag if enabled
if(DEBUG_CPU)
    target_compile_definitions(spaceinvaders PRIVATE DEBUG_CPU=1)
endif()

# Platform-specific settings
if(WIN32)
    # Windows: Link required system libraries
    target_link_libraries(spaceinvaders PRIVATE
        winmm
        imm32
        version
        setupapi
    )
    # Set subsystem to console
    set_target_properties(spaceinvaders PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
elseif(UNIX AND NOT APPLE)
    # Linux: Link required system libraries
    target_link_libraries(spaceinvaders PRIVATE
        m
        dl
        pthread
    )
endif()

# Copy input.cfg to Debug and Release directories
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/rsrc/input.cfg
    ${CMAKE_BINARY_DIR}/Debug/input.cfg
    COPYONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/rsrc/input.cfg
    ${CMAKE_BINARY_DIR}/Release/input.cfg
    COPYONLY
)
