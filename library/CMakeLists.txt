cmake_minimum_required(VERSION 3.1.0)

set(ROOT ${CMAKE_SOURCE_DIR}/jni)
set(PRELIB_ROOT ${CMAKE_SOURCE_DIR}/jni)

INCLUDE_DIRECTORIES(${PRELIB_ROOT}/libqrencode/include)
ADD_LIBRARY(qrencode
        SHARED
        IMPORTED)

SET_TARGET_PROPERTIES(qrencode
        PROPERTIES
        IMPORTED_LOCATION
        ${PRELIB_ROOT}/libqrencode/${ANDROID_ABI}/libqrencode.so)

ADD_LIBRARY(qrencode-android
        SHARED
        ${ROOT}/qrencode.cpp
        ${ROOT}/jni.cpp)

FIND_LIBRARY(log-lib
        log)

TARGET_LINK_LIBRARIES(qrencode-android
        qrencode
        ${log-lib})