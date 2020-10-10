cmake_minimum_required(VERSION 3.10)

include(GenerateExportHeader)

project(Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

find_package(SFML COMPONENTS graphics audio REQUIRED)

set(INCLUDE_FILE
    include/Export.hpp
    include/Renderer.hpp
    include/Map.hpp
    include/Settings.hpp
    include/Clock.hpp
    include/Skin.hpp
)
set(SOURCE_FILE
    src/Renderer/SpriteManip.hpp
    src/Renderer/SpriteManip.cpp
    src/Renderer/Renderer.cpp
    src/Map/Map.cpp
    src/Settings/Settings.cpp
    src/Clock/Clock.cpp
    src/Skin/Skin.cpp
    src/ANSIEscape/Color.cpp
)

add_library(
    RendererLib SHARED
        ${INCLUDE_FILE}
        ${SOURCE_FILE}
)

generate_export_header(RendererLib
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/Export.hpp
)
target_link_libraries(
    RendererLib 
        sfml-graphics 
        sfml-audio
)
target_include_directories(RendererLib 
    PUBLIC
        include
    PRIVATE
        src
)

find_package(nlohmann_json REQUIRED)

add_executable(RendererBin src/main.cpp)
target_link_libraries(RendererBin RendererLib nlohmann_json::nlohmann_json)