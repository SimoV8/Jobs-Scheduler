cmake_minimum_required(VERSION 3.3)
project(JobsScheduler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static")

set(SOURCE_FILES src/main.cpp src/DataContainer.cpp src/DataContainer.h src/Job.cpp src/Job.h src/Solver.cpp src/Solver.h src/Solution.cpp src/Solution.h src/RandomUtil.cpp src/RandomUtil.h src/Machine.cpp src/Machine.h src/Resource.cpp src/Resource.h)
add_executable(JobsScheduler ${SOURCE_FILES})