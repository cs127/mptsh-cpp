cmake_minimum_required(VERSION 3.18)
project(mptsh-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CLIPBOARDXX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../clipboardxx)

include_directories(${CLIPBOARDXX_DIR}/include)
add_executable(mptsh mptsh.cpp)
add_subdirectory(ClipboardXX)
target_link_libraries(mptsh ClipboardXX)
