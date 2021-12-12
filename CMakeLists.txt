cmake_minimum_required (VERSION 3.0)
project (CG-Firework)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

set(LIBS glfw3 opengl32 assimp freetype irrKlang)




file(GLOB SOURCE
            "src/*.h"
            "src/*.c"
            "src/*.hpp"
            "src/*.cpp")


set(NAME "CG-Firework")
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

include_directories(${CMAKE_SOURCE_DIR}/includes)

