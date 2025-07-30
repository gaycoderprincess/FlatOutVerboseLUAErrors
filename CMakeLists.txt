cmake_minimum_required(VERSION 3.27)
project(FlatOutVerboseLUAErrors)

SET(CMAKE_SYSTEM_NAME Windows)

SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
SET(CMAKE_RANLIB i686-w64-mingw32-ranlib)

SET(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -masm=intel -s -static")

add_compile_definitions(NYA_COMMON_NO_D3D)
add_library(FlatOutVerboseLUAErrors SHARED main.cpp)
target_include_directories(FlatOutVerboseLUAErrors PUBLIC ../nya-common)
target_include_directories(FlatOutVerboseLUAErrors PUBLIC ../nya-common/3rdparty)
target_include_directories(FlatOutVerboseLUAErrors PUBLIC ../nya-common-fo1)
target_link_options(FlatOutVerboseLUAErrors PRIVATE "-Wl,--exclude-all-symbols")
set_target_properties(FlatOutVerboseLUAErrors PROPERTIES PREFIX "")
set_target_properties(FlatOutVerboseLUAErrors PROPERTIES SUFFIX "_gcp.dll")