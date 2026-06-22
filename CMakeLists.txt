cmake_minimum_required(VERSION 3.10)

project(Edge-AI-Vision-Pipeline-Optimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED
    gstreamer-1.0
    gstreamer-base-1.0
    gstreamer-video-1.0
    gstreamer-audio-1.0
    gstreamer-app-1.0
)

add_executable(myapp ./Src/main.cpp)

target_include_directories(myapp PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
)

target_link_directories(myapp PRIVATE
    ${GSTREAMER_LIBRARY_DIRS}
)

target_link_libraries(myapp PRIVATE
    ${GSTREAMER_LIBRARIES}
)

target_compile_options(myapp PRIVATE
    ${GSTREAMER_CFLAGS_OTHER}
)
