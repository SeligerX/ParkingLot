add_library(
    vehicles
    vehicle.h
    vehicle.cpp
    bus.h
    bus.cpp
    car.h
    car.cpp
    motorcycle.h
    motorcycle.cpp
    )

include_directories(../utils)
target_include_directories(vehicles PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(vehicles PUBLIC utils)