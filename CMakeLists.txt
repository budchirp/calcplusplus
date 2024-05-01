cmake_minimum_required(VERSION 3.28)

set(EXEC_NAME "calcplusplus")
project(calcplusplus VERSION 0.1.0)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(ENABLE_SAMPLES OFF)
set(BUILD_TESTING OFF)
set(ENABLE_OPENMP OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(FTXUI_DEV_WARNINGS ON)
  
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(muparser
  GIT_REPOSITORY https://github.com/beltoforion/muparser
  GIT_TAG v2.3.4
)
FetchContent_GetProperties(muparser)
if(NOT muparser_POPULATED)
  FetchContent_Populate(muparser)
  add_subdirectory(${muparser_SOURCE_DIR} ${muparser_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

file(GLOB HEADERS
  "include/*.h"
  "include/*.hh"
  "include/*.hpp"
  "include/*.hxx"
)

file(GLOB SRC
  "src/*.c"
  "src/*.cc"
  "src/*.cpp"
  "src/*.cxx"
)

include_directories(include)

add_executable(${EXEC_NAME} ${SRC} ${HEADERS})
target_include_directories(${EXEC_NAME} PRIVATE ${HEADERS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_options(${EXEC_NAME} PRIVATE -fsanitize=address)
endif()

target_link_libraries(${EXEC_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component

  PRIVATE muparser::muparser
)
