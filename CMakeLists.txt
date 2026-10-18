cmake_minimum_required(VERSION 3.14)
project(TSPsolver)

# --- Global options ---
# Use C++20 standard, change if needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Debug/Release configuration ---
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- Compiler flags ---
if(MSVC)
  set(CMAKE_CXX_FLAGS "/W4 /EHsc")
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# --- Directories ---
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# --- Source variables ---
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB HEADERS "${INCLUDE_DIR}/*.hpp")

list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")

# --- Library ---
add_library(tsp_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(tsp_lib PUBLIC ${INCLUDE_DIR})

# --- Main executable ---
add_executable(TSPsolver ${SOURCE_DIR}/main.cpp)
target_link_libraries(TSPsolver PRIVATE tsp_lib)

# --- Tests ---
enable_testing()
file(GLOB TEST_SOURCES "${TEST_DIR}/*.cpp")
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE tsp_lib)
add_test(NAME tests COMMAND tests)
