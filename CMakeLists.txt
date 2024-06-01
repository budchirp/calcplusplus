cmake_minimum_required(VERSION 3.25)

project(calcplusplus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_compile_options(
  -Wall -Wextra -Wpedantic
)

file(GLOB HEADERS "include/*.h" "include/*.hpp" "include/*.hh" "include/*.hxx")
message(STATUS "Found headers: ${HEADERS}")
file(GLOB SOURCES "src/*.c" "src/*.cpp" "src/*.cc" "src/*.cxx")
message(STATUS "Found sources: ${SOURCES}")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
  target_compile_options(${PROJECT_NAME} PRIVATE -Og -g3 -DDEBUG)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native -flto -DNDEBUG)
  target_link_options(${PROJECT_NAME} PRIVATE -flto)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

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

target_link_libraries(${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component

  PRIVATE muparser::muparser
)
