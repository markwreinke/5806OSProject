cmake_minimum_required(VERSION 3.15)
project(5806OSProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(5806OSProject main.cpp src/StepZDebug.cpp include/StepZDebug.h src/UtilityUUID.cpp include/UtilityUUID.h src/ExEmulationTests.cpp include/ExEmulationTests.h src/Partition.cpp include/Partition.h src/Ext2File.cpp include/Ext2File.h src/Inode.cpp include/Inode.h src/FileAccess.cpp include/FileAccess.h src/Directories.cpp include/Directories.h src/FilePaths.cpp include/FilePaths.h src/copyFile.cpp include/copyFile.h)