cmake_minimum_required(VERSION 3.10)
project(NetWatch VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

# network library
add_library(network
  src/network/BoostSniffer.cpp
    src/parser/PacketParser.cpp
)
target_include_directories(network PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(network PUBLIC
  ${Boost_LIBRARIES}
  Threads::Threads
)

# storage library
add_library(storage
  src/storage/SQLiteStorage.cpp
)
target_include_directories(storage PUBLIC
  ${SQLite3_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(storage PUBLIC
  ${SQLite3_LIBRARIES}
)

# main executable
add_executable(NetWatch src/main.cpp)
target_include_directories(NetWatch PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(NetWatch PRIVATE
  network
  storage
  Threads::Threads
)
