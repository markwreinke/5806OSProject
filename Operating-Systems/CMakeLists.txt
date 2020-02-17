cmake_minimum_required(VERSION 3.15)
project(5806OSProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(5806OSProject main.cpp src/StepZDebug.cpp include/StepZDebug.h src/UtilityUUID.cpp include/UtilityUUID.h src/ExEmulationTests.cpp include/ExEmulationTests.h src/Partition.cpp include/Partition.h)