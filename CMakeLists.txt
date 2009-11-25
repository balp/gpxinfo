#
#
#
cmake_minimum_required(VERSION 2.6)
project (GPXSplitter)

set(CMAKE_CXX_FLAGS "-g -O3")
set(CMAKE_CXX_FLAGS_RELESE "-g -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")

set(SRCFILES
	MySAX2Handler.cpp
	track.cpp
)

add_library ( track 
	${SRCFILES}
)

add_executable ( gpxsplit 
	gpxsplit.cpp
)

target_link_libraries ( gpxsplit
	track
	xerces-c
)

add_executable ( gpxtimes
	gpxtimes.cpp
)

target_link_libraries ( gpxtimes
	track
	xerces-c
)

#add_test ( track ${RUNTIME_OUTPUT_DIRECTORY}/trackTest )

set(CXXTEST_INCLUDE_DIR /home/balp/src/cxxtest/cxxtest/)
set(CMAKE_MODULE_PATH cmake )
include ( cmake/FindCxxTest.cmake )
#find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(AFTER ${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(mytest mytest.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/track.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyTestSuite.h)
    target_link_libraries(mytest track) # as needed
endif(CXXTEST_FOUND)




