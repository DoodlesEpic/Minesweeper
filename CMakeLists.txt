cmake_minimum_required(VERSION 3.0)

project(Minesweeper VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(wxBUILD_SHARED OFF)
set(wxBUILD_USE_STATIC_RUNTIME ON)

set(wxWidgets_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/wxWidgets")
set(wxWidgets_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/wxWidgets/lib/vc_x64_lib")
set(wxWidgets_CONFIGURATION  "msw")

find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

add_executable(Minesweeper
  WIN32
  App.cpp
  MainGUI.cpp
)

target_link_libraries(Minesweeper ${wxWidgets_LIBRARIES})

install(TARGETS Minesweeper
  DESTINATION bin
)
