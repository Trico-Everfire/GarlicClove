cmake_minimum_required(VERSION 3.24)
project(GarlicClove)

set(CMAKE_CXX_STANDARD 20)

set(SRC
    library.cpp
    library.h
    logger.h
)

if(TEST_APPLICATION)
    add_executable(${PROJECT_NAME} main.cpp ${SRC})
else()
    add_library(GarlicClove STATIC ${SRC})
endif()


