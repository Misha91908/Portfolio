cmake_minimum_required(VERSION 3.6.1)
project(Graph_Editor)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/app/launcher.cpp src/app/widgets/graphs.cpp src/app/editorwindow.cpp src/app/project/project.cpp src/app/project/serialization.cpp src/app/project/serialization.h src/app/widgets/lineparameters.cpp src/app/widgets/lineparameters.h)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)


add_executable(Graph_Editor ${SOURCE_FILES})

target_link_libraries(Graph_Editor Qt5::Widgets Qt5::Charts)
