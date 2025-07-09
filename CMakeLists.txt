cmake_minimum_required(VERSION 3.24)
project(TeaDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_CXX_FLAGS "-O2")

if(WIN32)
        set(SQLite3_PATH D:/sqlite3)
        set(SQLite3_INCLUDE_DIR ${SQLite3_PATH}/include)
        set(SQLite3_LIBRARY_DIR ${SQLite3_PATH}/lib)
        set(SQLite3_LIBRARY sqlite3.dll)
        set(CMAKE_WIN32_EXECUTABLE ON)
        set(CMAKE_INSTALL_PATH "TeaDB")
        if(MSVC)
                set(QT_PATH "D:/Qt/6.9.0/msvc2022_64")
                add_compile_options("/permissive-")
                add_compile_options("/Zc:__cplusplus")
        else()
                set(QT_PATH "D:/Qt/6.9.0/mingw_64")
                set(MINGW_PATH "D:/mingw64")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
        endif()
elseif(APPLE)
        set(QT_PATH "/Users/zhangshengcheng/Qt/6.6.0/macos")
else()
        set(QT_PATH "/home/zsc/Qt/6.10.0/gcc_64")
endif()

set(CMAKE_PREFIX_PATH ${QT_PATH})

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
        src/qwaiting.ui
        resources/logo.rc)
if(LINUX)
  target_link_libraries(TeaDB
          sqlite3
          Qt::Widgets
  )
else()
  target_link_libraries(TeaDB
          SQLite3
          Qt::Widgets
  )
endif()

if(WIN32)
        target_include_directories(TeaDB PRIVATE ${SQLite3_INCLUDE_DIRS})
        target_link_directories(TeaDB PRIVATE ${SQLite3_LIBRARY_DIR})
        target_link_libraries(TeaDB ${SQLite3_LIBRARIES})
endif()

if(WIN32)
        set(TEADB_DIR ${CMAKE_BINARY_DIR}/TeaDB)
        install(TARGETS TeaDB DESTINATION ${TEADB_DIR})
        install(DIRECTORY resources DESTINATION ${TEADB_DIR})
        install(FILES "${QT_PATH}/bin/Qt6Core.dll"
                "${QT_PATH}/bin/Qt6Gui.dll"
                "${QT_PATH}/bin/Qt6Widgets.dll"
                "${SQLite3_LIBRARY_DIR}/sqlite3.dll"
                DESTINATION ${TEADB_DIR})
        if(MINGW)
                install(FILES "${QT_PATH}/bin/libstdc++-6.dll" DESTINATION ${TEADB_DIR})
                install(FILES "${MINGW_PATH}/bin/libssp-0.dll" DESTINATION ${TEADB_DIR})
        endif()
        install(FILES "${QT_PATH}/plugins/imageformats/qgif.dll" DESTINATION "${TEADB_DIR}/imageformats")
        install(FILES "${QT_PATH}/plugins/platforms/qwindows.dll" DESTINATION "${TEADB_DIR}/platforms")
endif()
