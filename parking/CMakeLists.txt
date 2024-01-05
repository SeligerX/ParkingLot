add_library(
    parking
    parking.h
    parking.cpp
    )

include_directories(../utils)
include_directories(../vehicles)
target_include_directories(parking PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(parking PUBLIC utils vehicles)