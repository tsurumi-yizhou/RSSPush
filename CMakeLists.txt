cmake_minimum_required(VERSION 3.26)
project(RSSPush)

set(CMAKE_CXX_STANDARD 23)
set(ENABLE_SQLITE3 ON)
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

include_directories(source)
file(GLOB_RECURSE SRCS source/*.cpp source/*.hpp source/*.cppm)
add_executable(main ${SRCS})

add_subdirectory(third_party/ormpp)
include_directories(third_party/ormpp/include)
include_directories(third_party/ormpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

find_package(fawdlstty-libfv CONFIG REQUIRED)
target_link_libraries(main PRIVATE fawdlstty-libfv::libfv)

find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
target_include_directories(main PRIVATE ${JWT_CPP_INCLUDE_DIRS})

find_package (asio CONFIG REQUIRED)
target_link_libraries (main PRIVATE asio asio::asio)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)

find_path (GZIP_HPP_INCLUDE_DIRS "gzip/compress.hpp")
target_include_directories (main PRIVATE ${GZIP_HPP_INCLUDE_DIRS})

find_package (OpenSSL REQUIRED)
target_link_libraries (main PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package (ZLIB REQUIRED)
target_link_libraries (main PRIVATE ZLIB::ZLIB)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE unofficial::sqlite3::sqlite3)

find_package(tinyxml2 CONFIG REQUIRED)
target_link_libraries(main PRIVATE tinyxml2::tinyxml2)