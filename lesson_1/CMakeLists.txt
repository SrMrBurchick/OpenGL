cmake_minimum_required(VERSION 3.10.2)

set(PROJECT_NAME "OpenGL_Lesson_1")
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory(./External/glad)
include_directories(./External/glad/include)

add_compile_definitions(VERTEX_SHADER_PATH="${PROJECT_SOURCE_DIR}/Sources/Private/VertexShader.glsl")
add_compile_definitions(FRAGMENT_SHADER_PATH="${PROJECT_SOURCE_DIR}/Sources/Private/FragmentShader.glsl")

add_executable(${PROJECT_NAME} ./Sources/Private/Lesson1.cpp)
target_link_libraries(${PROJECT_NAME} glfw GL glad)
