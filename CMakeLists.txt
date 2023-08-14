cmake_minimum_required(VERSION 3.18)

# EDIT HERE
set(EXEC_NAME "calcplusplus")
project(calcplusplus VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
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

FetchContent_MakeAvailable(muparser)

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

target_include_directories(${EXEC_NAME} PUBLIC ${HEADERS})
target_link_libraries(${EXEC_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component

  PRIVATE muparser::muparser
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Werror -fsanitize=undefined -fsanitize=address")
  target_link_options(${EXEC_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
