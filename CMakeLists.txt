cmake_minimum_required(VERSION 3.16)

project(PETDataAnalysis LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ROOT
find_package(ROOT REQUIRED)

# Create executable
add_executable(PETDataAnalysis src/PlotMaPMTData.cpp)

# Add include directory for your header
target_include_directories(PETDataAnalysis PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Include ROOT headers
target_include_directories(PETDataAnalysis PRIVATE ${ROOT_INCLUDE_DIRS})

# Link ROOT libraries
target_link_libraries(PETDataAnalysis PRIVATE ${ROOT_LIBRARIES})

target_compile_definitions(PETDataAnalysis PRIVATE ${ROOT_DEFINITIONS})