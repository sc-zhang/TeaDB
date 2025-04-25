cmake_minimum_required(VERSION 3.24)
project(TeaDB)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_PREFIX_PATH "/Users/zhangshengcheng/Qt/6.6.0/macos")

find_package(SQLite3)
find_package(Qt6 COMPONENTS
        Widgets
        REQUIRED)

add_executable(TeaDB src/main.cpp
        src/teadbgui.h
        src/ui_teadbgui.h
        src/teadbgui.cpp
        src/query_db.cpp
        src/query_db.h
        src/qwaiting.cpp
        src/qwaiting.h
        src/qwaiting.ui)
target_link_libraries(TeaDB
        SQLite3
        Qt::Widgets
)

