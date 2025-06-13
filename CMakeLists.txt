cmake_minimum_required(VERSION 3.10)
project(Learning2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)

# Add GLFW as a CMake subproject
add_subdirectory(include/glfw)

add_executable(Learning2
    src/main.cpp
    src/glad.c
    src/stb_image.cpp
)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/imgui
    ${PROJECT_SOURCE_DIR}/include/imgui/backends
    ${PROJECT_SOURCE_DIR}/include/glfw/include
)

target_include_directories(Learning2 PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src            # your src/
    ${PROJECT_SOURCE_DIR}/include/imgui        # ImGui core
    ${PROJECT_SOURCE_DIR}/include/imgui/backends # ImGui GLFW/OpenGL backends
    ${PROJECT_SOURCE_DIR}/include/glfw/include # GLFW public headers
)

# Set output paths based on build directory

# Link with glfw and system OpenGL  
if(WIN32)
    target_link_libraries(Learning2 PRIVATE
        glfw
        imgui
        opengl32         # Windows

    )   
elseif(APPLE)
    # macOS specifics if you ever go that route
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(Learning2 PRIVATE
        glfw
        imgui
        ${OpenGL_LIBRARY}
        ${COCOA_LIBRARY}
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Learning2 PRIVATE
        glfw
        imgui
        OpenGL::GL      # UNIX
    ) 
endif()

#file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/src/shaders/*.glsl")

#slightly more convoluted way of copying the shader files whenever the project is built
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

file(GLOB TEXTURE_FILES "${CMAKE_SOURCE_DIR}/textures/*")

#slightly more convoluted way of copying the shader files whenever the project is built
add_custom_target(copy_textures ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/textures
)

foreach(TEXTURE_FILE ${TEXTURE_FILES})
    get_filename_component(TEXTURE_NAME ${TEXTURE_FILE} NAME)
    add_custom_command(
        TARGET copy_textures POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${TEXTURE_FILE}
                ${CMAKE_BINARY_DIR}/textures/${TEXTURE_NAME}
    )
endforeach()