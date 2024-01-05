cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

project(parkingDemo VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CTest)
enable_testing()

add_subdirectory(utils)
add_subdirectory(vehicles)
add_subdirectory(parking)
add_subdirectory(tests)

add_executable(parkingDemo parkingDemo.cpp)
target_link_libraries(parkingDemo PRIVATE utils vehicles parking)

# multithreading deom
add_executable(parkingMTDemo parkingMTDemo.cpp)
target_link_libraries(parkingMTDemo PRIVATE utils vehicles parking)