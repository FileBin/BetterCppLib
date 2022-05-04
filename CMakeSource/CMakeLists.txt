cmake_minimum_required (VERSION 3.8)

#project config

project (BetterCpp)

set(BetterCpp_PATH "${CMAKE_SOURCE_DIR}")

set(LIB_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/lib/include")
set(LIB_BIN_PATH "${CMAKE_SOURCE_DIR}/lib/bin")

set(LINKER_FLAGS "")

if(WIN32)
    set(LIB_BIN_PATH "${LIB_BIN_PATH}/win64")
endif(WIN32)
if(UNIX)
    set(LINKER_FLAGS "-no-pie")
    set(LIB_BIN_PATH "${LIB_BIN_PATH}/linux64")
endif(UNIX)

set(ANY_LIB_BIN_PATH "${LIB_BIN_PATH}/any")

message("lib any path: ${ANY_LIB_BIN_PATH}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -D UNICODE ${LINKER_FLAGS}")
  	set(LIB_BIN_PATH "${LIB_BIN_PATH}/clang")
  	
  	
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -D UNICODE ${LINKER_FLAGS}")
  	set(LIB_BIN_PATH "${LIB_BIN_PATH}/gnu")
  	
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  	set(LIB_BIN_PATH "${LIB_BIN_PATH}/i8")
  	
  	
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(/W4 /WX)
  	set(LIB_BIN_PATH "${LIB_BIN_PATH}/msvc")
endif()

message("lib compiler path: ${LIB_BIN_PATH}")

set(RESOURCES_PATH "${CMAKE_SOURCE_DIR}/res/")

#file search

file(GLOB_RECURSE BetterCpp_SRC 
"${BetterCpp_PATH}/*.h" "${BetterCpp_PATH}/*.c" 
"${BetterCpp_PATH}/*.hpp" "${BetterCpp_PATH}/*.cpp")

message("sources: ${BetterCpp_SRC}")


file(GLOB_RECURSE DLLs_x64 "${CMAKE_SOURCE_DIR}/lib/*.dll")

file(GLOB_RECURSE RESOURCES "${RESOURCES_PATH}/*.*")


if(MSVC)
    add_compile_options(/WX-)
    add_link_options(/WX-)
endif()

#Source config

#BetterCpp


include_directories(BetterCpp ${LIB_INCLUDE_PATH})
include_directories(BetterCpp "${BetterCpp_PATH}/include")
include_directories(BetterCpp "${BetterCpp_PATH}/src")

#build
add_library (BetterCpp ${BetterCpp_SRC})
target_precompile_headers(BetterCpp PUBLIC "${BetterCpp_PATH}/src/pch.hpp")

#properties
set_target_properties(BetterCpp PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_CXX_STANDARD 20)


if(WIN32)
#copy dll if not empty list
if (NOT "${DLLs_x64}" STREQUAL "")
    add_custom_command(TARGET BetterCpp POST_BUILD   # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND}
            -E copy_if_different                    # which executes "cmake - E copy_if_different..."
            ${DLLs_x64}                           # <--this is in-file
            ${CMAKE_BINARY_DIR})                  # <--this is out-file path
    endif()
endif(WIN32)

#copy resources
add_custom_target(Resources ALL DEPENDS ${RESOURCES})
set(OUT_RES_DIR ${CMAKE_BINARY_DIR}/resources)

file(MAKE_DIRECTORY ${OUT_RES_DIR})
if(EXISTS ${RESOURCES_PATH})
    add_custom_command(TARGET Resources   # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory                    # which executes "cmake - E copy_if_different..."
        ${RESOURCES_PATH}                           # <--this is in-file
        ${OUT_RES_DIR})          # <--this is out-file path
endif()
