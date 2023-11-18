cmake_minimum_required(VERSION 3.21)
project(kmeans)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(kmeans main.cpp Points.cpp Centroids.cpp Kmeans.cpp)
