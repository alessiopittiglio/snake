project(Snake)

# CMake 3.* necessario per Qt
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# ottengo tutti i sorgenti
file(GLOB my_sources *.h *.cpp *.qrc *.ui)

# attivo funzionalita' specifiche di CMake per Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# attiva C++11
add_definitions(-std=gnu++0x)

# aggiungo eseguibile
add_executable(Snake ${my_sources})

# verifico l'esistenza delle librerie Qt necessarie
find_package(Qt5Widgets REQUIRED)

# collego le librerie Qt con l'eseguibile
target_link_libraries(Snake Qt5::Widgets)