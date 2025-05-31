cmake_minimum_required(VERSION 3.10)
project(SpaceShooter)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler to MinGW
set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")

# Set SDL2 paths for Windows
set(SDL2_DIR "C:/Development/SDL2/x86_64-w64-mingw32")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include/SDL2")
set(SDL2_LIBRARY "${SDL2_DIR}/lib/libSDL2.dll.a")
set(SDL2MAIN_LIBRARY "${SDL2_DIR}/lib/libSDL2main.a")

# Include directories
include_directories(${SDL2_INCLUDE_DIR}
                  ${CMAKE_SOURCE_DIR}/src)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
                     ${SDL2_LIBRARY}
                     ${SDL2MAIN_LIBRARY}
                     ${SDL2_DIR}/lib/libSDL2_ttf.dll.a
                     ${SDL2_DIR}/lib/libSDL2_mixer.dll.a
                     ${SDL2_DIR}/lib/libSDL2_image.dll.a)

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/img DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/audio DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/ttf DESTINATION ${CMAKE_BINARY_DIR}/Debug)

file(COPY ${CMAKE_SOURCE_DIR}/img DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY ${CMAKE_SOURCE_DIR}/audio DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY ${CMAKE_SOURCE_DIR}/ttf DESTINATION ${CMAKE_BINARY_DIR}/Release)

# Copy SDL2 DLLs to build directory
file(COPY "${SDL2_DIR}/bin/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY "${SDL2_DIR}/bin/SDL2_ttf.dll" DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY "${SDL2_DIR}/bin/SDL2_mixer.dll" DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY "${SDL2_DIR}/bin/SDL2_image.dll" DESTINATION ${CMAKE_BINARY_DIR}/Debug)

# Copy SDL2 DLLs to release directory
file(COPY "${SDL2_DIR}/bin/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY "${SDL2_DIR}/bin/SDL2_ttf.dll" DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY "${SDL2_DIR}/bin/SDL2_mixer.dll" DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY "${SDL2_DIR}/bin/SDL2_image.dll" DESTINATION ${CMAKE_BINARY_DIR}/Release)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 