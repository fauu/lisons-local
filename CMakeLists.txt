cmake_minimum_required(VERSION 3.11)

project(lisons-local)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ";" ${QT5_PACKAGE_DIR})

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")

set(SOURCES
    lib/hobrasofthttp/httpconnection.cpp
    lib/hobrasofthttp/httpcookie.cpp
    lib/hobrasofthttp/httpgzipcompression.cpp
    lib/hobrasofthttp/httprequest.cpp
    lib/hobrasofthttp/httprequesthandler.cpp
    lib/hobrasofthttp/httpresponse.cpp
    lib/hobrasofthttp/httpserver.cpp
    lib/hobrasofthttp/httpsession.cpp
    lib/hobrasofthttp/httpsessionstore.cpp
    lib/hobrasofthttp/httpsettings.cpp
    lib/hobrasofthttp/httptcpserver.cpp
    lib/hobrasofthttp/shtmlcontroller.cpp
    lib/hobrasofthttp/staticfilecontroller.cpp
    src/main.cpp
    src/dist_manager.cpp
    src/backend.cpp
    src/file_md5.cpp
)

set(HEADERS
    lib/hobrasofthttp/httpconnection.h
    lib/hobrasofthttp/httpcookie.h
    lib/hobrasofthttp/httpgzipcompression.h
    lib/hobrasofthttp/httprequest.h
    lib/hobrasofthttp/httprequesthandler.h
    lib/hobrasofthttp/httpresponse.h
    lib/hobrasofthttp/httpserver.h
    lib/hobrasofthttp/httpsession.h
    lib/hobrasofthttp/httpsessionstore.h
    lib/hobrasofthttp/httpsettings.h
    lib/hobrasofthttp/httptcpserver.h
    lib/hobrasofthttp/shtmlcontroller.h
    lib/hobrasofthttp/staticfilecontroller.h
    src/dist_manager.h
    src/backend.h
    src/file_md5.h
)

# add_definitions(${QtQml_DEFINITIONS})

# include_directories(${QtQml_INCLUDE_DIRS})

#qt5_add_resources(qml_QRC qml.qrc)

#file(GLOB sources
#        "lib/hobrasofthttp/*.h"
#        "lib/hobrasofthttp/*.cpp"
#        "src/*.h"
#        "src/*.cpp"
#        "qml.qrc"
#)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} qml.qrc)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)

