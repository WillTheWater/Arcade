cmake_minimum_required(VERSION 3.30.3)
project(Arcade CXX)

set(SFML_BUILD_NETWORK OFF)
set(SPDLOG_USE_STD_FORMAT ON)
set(TGUI_BACKEND SFML_GRAPHICS)

include(FetchContent)
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_Declare(logs URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz)
FetchContent_Declare(tgui URL https://github.com/texus/TGUI/archive/refs/tags/v1.11.0.tar.gz)
FetchContent_MakeAvailable(sfml json logs tgui)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Include/*.h
)

add_executable(Arcade
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(Arcade PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Arcade>"
)

target_include_directories(Arcade PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Include
)

target_compile_features(Arcade PRIVATE cxx_std_20)
target_compile_options(Arcade PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>, /W4, -Wall -Wextra>)

target_link_libraries(Arcade PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Main
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    TGUI::TGUI
)

add_custom_command(TARGET Arcade PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Content
            $<TARGET_FILE_DIR:Arcade>/Content
)