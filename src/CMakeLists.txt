cmake_minimum_required (VERSION 2.6)
project (ConstraintRelationships)

# project source and build directories

# The version number
set (ConstraintRelationships_VERSION_MAJOR 1)
set (ConstraintRelationships_VERSION_MINOR 0)

SET(GECODE_LIBRARIES gecodesearch gecodeint gecodekernel gecodesupport gecodeminimodel gecodeset )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

if (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
	add_definitions (-DCMAKE_CXX_COMPILER_ARG1=-std=c++11)
	add_definitions (-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE)
endif (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")

include_directories("${PROJECT_BINARY_DIR}")

# add the actual executable
add_executable(ConstraintRelationships crtest.cpp)
target_link_libraries(ConstraintRelationships ${GECODE_LIBRARIES})
 
#target_link_libraries (Tutorial ${EXTRA_LIBS})
