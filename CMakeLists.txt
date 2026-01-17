cmake_minimum_required(VERSION 3.30.3)
project(Arcade CXX)

set(SFML_BUILD_NETWORK OFF)

include(FetchContent)
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)
FetchContent_MakeAvailable(sfml)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/*.cpp
)

add_executable(Arcade ${SOURCES})
target_include_directories(Arcade PRIVATE Engine/Include)
target_compile_features(Arcade PRIVATE cxx_std_20)
target_compile_options(Arcade PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>, /W4, -Wall -Wextra>)
target_link_libraries(Arcade PRIVATE SFML::Graphics SFML::Audio SFML::Main)

add_custom_command(TARGET Arcade POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:Arcade>/Assets
)
