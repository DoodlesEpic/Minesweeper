cmake_minimum_required(VERSION 3.0)

project(Minesweeper VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(wxWidgets 3.0 REQUIRED COMPONENTS std stc)

add_executable(Minesweeper
  App.cpp
  MainGUI.cpp
)

target_link_libraries(Minesweeper
  PRIVATE
    wx::std
    wx::stc
)

install(TARGETS Minesweeper
  DESTINATION bin
)
