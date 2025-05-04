cmake_minimum_required(VERSION 3.10)
project(Learning2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GLFW as a CMake subproject
add_subdirectory(include/glfw)

add_executable(Learning2
    src/main.cpp
    src/glad.c
)

target_include_directories(Learning2 PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Set output paths based on build directory

# Link with glfw and system OpenGL  
if(WIN32)
    target_link_libraries(Learning2 PRIVATE
        glfw
        opengl32         # Windows
    )   
elseif(APPLE)
    # macOS specifics if you ever go that route
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(Learning2 PRIVATE
        glfw
        ${OpenGL_LIBRARY}
        ${COCOA_LIBRARY}
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Learning2 PRIVATE
        glfw
        OpenGL::GL      # UNIX
    ) 
endif()

#file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/src/shaders/*.glsl")

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    add_custom_command(
        TARGET copy_shaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SHADER_FILE}
                ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}
    )
endforeach()