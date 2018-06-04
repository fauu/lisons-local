cmake_minimum_required(VERSION 3.11)

project(lisons-local)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ";" ${QT5_PACKAGE_DIR})

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")

file(GLOB SOURCES
    "lib/hobrasofthttp/*.h"
    "lib/hobrasofthttp/*.cpp"
    "src/*.h"
    "src/*.cpp"
    "res/res.qrc"
)

set(FORMS
    res/ui/main_window.ui
)
qt5_wrap_ui(FORM_H ${FORMS})

add_executable(${PROJECT_NAME} ${SOURCES} ${FORM_H})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Network)

