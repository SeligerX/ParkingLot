add_library(
    utils
    privateAccessor.h
    money.h
    money.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")