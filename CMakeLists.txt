cmake_minimum_required(VERSION 3.10)

project(UltraMinecraftClone)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
#find_package(OpenGL REQUIRED) TODO! Add this if we want to build for Emscripten
find_package(glm REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(game ${SRC_FILES})

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets directory"
)

target_compile_features(game PRIVATE cxx_std_20)

# TEMPORARY!!!!!
add_subdirectory("Dependencies/RmlUi-6.2")

target_link_libraries(game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    #OpenGL::GL    TODO! Add this if we want to build for Emscripten
    glm::glm
)
target_compile_definitions(game PRIVATE RMLUI_SDL_VERSION_MAJOR=3 SDL_MAJOR_VERSION=3)
target_link_libraries(game PRIVATE RmlUi::RmlUi)
