cmake_minimum_required(VERSION 3.10)
project(i8080-spaceinvaders-emulator C)

set(CMAKE_C_STANDARD 11)

# Debug option for CPU tracing
option(DEBUG_CPU "Enable CPU instruction tracing and register dumps" OFF)

# Add SDL2 from submodule
set(SDL_SHARED OFF CACHE BOOL "Build SDL2 as static library")
set(SDL_STATIC ON CACHE BOOL "Build SDL2 as static library")
add_subdirectory(external/SDL2)

# Source files
set(SOURCES
    src/main.c
    src/emulator.c
    src/i8080.c
    src/opcodes.c
    src/memory.c
    src/io.c
    src/input.c
    src/platform.c
    src/display.c
    src/disassembler.c
    src/hexdump.c
)

# Executable
add_executable(intel8080 ${SOURCES})

# Include directories
target_include_directories(intel8080 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/roms
)

# Link SDL2
target_link_libraries(intel8080 PRIVATE SDL2::SDL2-static SDL2::SDL2main)

# Add debug flag if enabled
if(DEBUG_CPU)
    target_compile_definitions(intel8080 PRIVATE DEBUG_CPU=1)
endif()

# Platform-specific settings
if(WIN32)
    # Windows: Link required system libraries
    target_link_libraries(intel8080 PRIVATE
        winmm
        imm32
        version
        setupapi
    )
    # Set subsystem to console
    set_target_properties(intel8080 PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
elseif(UNIX AND NOT APPLE)
    # Linux: Link required system libraries
    target_link_libraries(intel8080 PRIVATE
        m
        dl
        pthread
    )
endif()

# Copy ROM files to build directory (if needed)
file(GLOB ROM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/roms/*.rom")
foreach(ROM_FILE ${ROM_FILES})
    get_filename_component(ROM_NAME ${ROM_FILE} NAME)
    configure_file(${ROM_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${ROM_NAME} COPYONLY)
endforeach()
