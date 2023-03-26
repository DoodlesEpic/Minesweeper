cmake_minimum_required(VERSION 3.0)

project(Minesweeper VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(wxBUILD_SHARED OFF)
if (WIN32)
  set(wxBUILD_USE_STATIC_RUNTIME ON)
endif()

add_subdirectory(libs/wxWidgets)
add_executable(Minesweeper
  App.cpp
  MainGUI.cpp
)

target_link_libraries(Minesweeper wx::core wx::base)

install(TARGETS Minesweeper
  DESTINATION bin
)
