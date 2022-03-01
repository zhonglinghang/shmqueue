cmake_minimum_required(VERSION 3.10)

project(shmqueue)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -ffunction-sections")

set(CMAKE_C_STANDARD 11)

include_directories(shmqueue)
aux_source_directory(shmqueue SOURCE_DIRECTORY)

if (UNIX AND NOT APPLE)
  list(APPEND libs rt pthread)
endif ()

add_compile_options(-march=native)
if(SINGLE_H)
  add_definitions(-DSINGLE_HEADER)
ENDIF(SINGLE_H)

add_executable(shmrunner test/shmrunner.c ${SOURCE_DIRECTORY})
add_executable(shmpublisher test/shmpublisher.c ${SOURCE_DIRECTORY})
add_executable(shmsubscriber test/shmsubscriber.c ${SOURCE_DIRECTORY})

add_library(libshmqueue STATIC ${SOURCE_DIRECTORY})
target_link_libraries(shmrunner ${libs})
