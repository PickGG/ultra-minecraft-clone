cmake_minimum_required(VERSION 3.10)

project(UltraMinecraftClone)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
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

target_link_libraries(game PRIVATE
    SDL3::SDL3
    OpenGL::GL
    glm::glm
)
