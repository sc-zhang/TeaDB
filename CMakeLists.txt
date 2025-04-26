cmake_minimum_required(VERSION 3.24)
project(TeaDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.0/msvc2022_64")

if(WIN32)
        set(SQLite3_INCLUDE_DIR D:/sqlite3/include)
        set(SQLite3_LIBRARY_DIR D:/sqlite3/lib)
        set(SQLite3_LIBRARY sqlite3.dll)
        set(CMAKE_WIN32_EXECUTABLE ON)
        add_compile_options("/permissive-")
        add_compile_options("/Zc:__cplusplus")
endif()

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
        src/ui_qwaiting.h
        src/qwaiting.cpp
        src/qwaiting.h
        src/qwaiting.ui)
target_link_libraries(TeaDB
        SQLite3
        Qt::Widgets
)

target_include_directories(TeaDB PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_directories(TeaDB PRIVATE ${SQLite3_LIBRARY_DIR})
target_link_libraries(TeaDB ${SQLite3_LIBRARIES})

