cmake_minimum_required(VERSION 2.8.12)
project(bitset)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Поиск Qt4
find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

# Исходные файлы
set(SOURCES
    main.cpp
    mainwindow.cpp
    bitset.cpp
)

set(HEADERS
    mainwindow.h
    bitset.h
)

# Создание исполняемого файла
add_executable(bitset ${SOURCES} ${HEADERS})

# Линкование с Qt
target_link_libraries(bitset ${QT_LIBRARIES})

# Флаги компилятора
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
