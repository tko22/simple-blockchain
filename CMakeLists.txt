cmake_minimum_required(VERSION 3.10)
project(Blockchain)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)

include_directories(core network utilities)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
  OpenSSL::SSL
  OpenSSL::Crypto
  Boost::boost
  Boost::system
)