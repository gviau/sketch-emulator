cmake_minimum_required(VERSION 2.8)

project(sketch-emulator)

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")

set (SOURCE_FILES
	src/main.cpp
	src/CPU.cpp
	src/InterruptsHandler.cpp
	src/Memory.cpp
)

set (HEADER_FILES
	include/CPU.h
	include/InterruptsHandler.h
	include/Memory.h
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(
	sketch-emulator
	${SOURCE_FILES}
	${HEADER_FILES}
)