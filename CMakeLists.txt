cmake_minimum_required(VERSION 3.26)
project(RSSPush)

set(CMAKE_CXX_STANDARD 23)

include_directories(source)
file(GLOB_RECURSE SRCS source/*.cpp source/*.hpp source/*.cppm)
add_executable(main ${SRCS})

find_package(Boost REQUIRED system)
target_link_libraries(main PRIVATE Boost::boost Boost::system )

find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
target_include_directories(main PRIVATE ${JWT_CPP_INCLUDE_DIRS})