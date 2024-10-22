cmake_minimum_required(VERSION 3.4)
project(sparse-passes)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g")        #/MT
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra") 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# set(LLVM_PROJECT_DIR "/mnt/ssd2/dsk/sparse-compiler/llvm-project" )
# set(LLVM_BUILD_DIR "/mnt/ssd2/dsk/sparse-compiler/llvm-project/build-17" )
# set(LLVM_BUILD_DIR "/mnt/ssd2/dsk/llvm-17.0.0-debug")

# include_directories("${LLVM_PROJECT_DIR}/llvm/include")
if( LLVM_INSTALL_PREFIX STREQUAL "" )
	MESSAGE(FATAL_ERROR "LLVM_INSTALL_PREFIX is empty")
else()
	MESSAGE("LLVM_INSTALL_PREFIX=${LLVM_INSTALL_PREFIX}")
endif()

include_directories(SYSTEM "${LLVM_INSTALL_PREFIX}/include")
include_directories("./include")
link_directories("${LLVM_INSTALL_PREFIX}/lib")
# aux_source_directory(./src src_1)
# set(srcs ${src_1})

# add_library(mypasses SHARED ${srcs})
add_library(Instrumentation SHARED src/instrumentation.cpp)
add_library(probe SHARED src/probe.cpp)


target_link_libraries(probe PRIVATE stdc++fs)
set_target_properties(probe PROPERTIES LINK_FLAGS "-static-libstdc++ -static-libgcc -fvisibility=hidden -fvisibility-inlines-hidden -Wl,--version-script=${PROJECT_SOURCE_DIR}/src/version.script")






