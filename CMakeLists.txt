cmake_minimum_required(VERSION 3.11)

project(lisons-local)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

file(GLOB SOURCES
    "lib/hobrasofthttp/*.h"
    "lib/hobrasofthttp/*.cpp"
    "src/*.h"
    "src/*.cpp"
    "res/res.qrc"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)

