cmake_minimum_required(VERSION 3.16)
project(programmable_nametag)

set(CMAKE_CXX_STANDARD 20)
include(CheckIncludeFileCXX)

include_directories(SYSTEM ${CMAKE_SYSROOT}/usr/include/freetype2)
include_directories(SYSTEM include)
include_directories(SYSTEM include/nlohmann/detail)
include_directories(SYSTEM include/uWebSockets/src)
include_directories(SYSTEM include/uWebSockets/uSockets/src)
include_directories(source)
include_directories(source/drawers)
include_directories(source/net)
include_directories(source/util)
include_directories(source/wrappers)

set(CMAKE_CXX_FLAGS "-pipe -Winvalid-pch -fexceptions -Wno-psabi -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -ggdb -Wall -Wextra -Wformat -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-qual  -Wno-unused-parameter -Wfatal-errors -DDEBUGGING -Wsuggest-final-types -Wpedantic -Wnull-dereference -fno-omit-frame-pointer -fdiagnostics-color -ftemplate-depth=64 -fconstexpr-depth=64 -ftemplate-backtrace-limit=8 -Wreorder -Wold-style-cast -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard")

set(SOURCE_FILES
        source/drawers/drawable.hpp
        source/drawers/text.cpp
        source/drawers/text.hpp
        source/drawers/sprite.cpp
        source/drawers/sprite.hpp
        source/drawers/stb_image.cpp
        source/drawers/texture.cpp
        source/drawers/texture.hpp
        source/drawers/triangle.cpp
        source/drawers/triangle.hpp
        source/drawers/vertex.hpp
        source/net/server.cpp
        source/net/server.hpp
        source/util/animationController.cpp
        source/util/animationController.hpp
        source/util/io.cpp
        source/util/io.hpp
        source/util/resourceHandler.cpp
        source/util/resourceHandler.hpp
        source/util/time.cpp
        source/util/time.hpp
        source/wrappers/driver.cpp
        source/wrappers/driver.hpp
        source/wrappers/egl.cpp
        source/wrappers/egl.hpp
        source/wrappers/hardware.cpp
        source/wrappers/hardware.hpp
        source/wrappers/openGL.cpp
        source/wrappers/openGL.hpp
        source/wrappers/shader.cpp
        source/wrappers/shader.hpp
        source/main.cpp)

add_executable(nametag ${SOURCE_FILES})

CHECK_INCLUDE_FILE_CXX("bcm2835.h" HAVE_BCM2835 "-I${PREFIX}/include")
if (${HAVE_BCM2835})
    include_directories(SYSTEM ${CMAKE_SYSROOT}/opt/vc/include)
    target_link_directories(nametag PUBLIC ${CMAKE_SYSROOT}/opt/vc/lib)
    target_link_libraries(nametag PUBLIC bcm2835 brcmEGL brcmGLESv2 freetype rt z)
else ()
    target_link_libraries(nametag PUBLIC freetype rt SDL2 GLESv2 z)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEV_MODE")
endif ()

find_library(USOCKETS_LIB uSockets.a HINT include/uWebSockets/uSockets)
target_link_libraries(nametag PUBLIC ${USOCKETS_LIB})

