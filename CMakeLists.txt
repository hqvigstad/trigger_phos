project(trigger)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# ROOT 
find_package(ROOT REQUIRED)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})
set(LIBS ${LIBS} ${ROOT_LIBRARIES})

#AliROOT
find_package(ALIROOT REQUIRED PHOS RAW)
message(${ALIROOT_INCLUDE_DIRS})
include_directories(SYSTEM ${ALIROOT_INCLUDE_DIRS})
set(LIBS ${LIBS} ${ALIROOT_LIBRARIES})

# Build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Woverloaded-virtual -Weffc++ -Wctor-dtor-privacy -std=c++0x")

# clang
#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_CXX_FLAGS -Weverything)

add_library(libTrigger 
		       AliTRUPedestalAnalysis.cxx  
		       AliTRUPedestalOutput.cxx
		       )
