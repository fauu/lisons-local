#!/bin/bash
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_PREFIX_PATH="C:\Qt\5.11.0\mingw53_32" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make -j 2

# TODO:
# windeployqt --release --qmldir "..\res\ui" .
# openssl dlls
