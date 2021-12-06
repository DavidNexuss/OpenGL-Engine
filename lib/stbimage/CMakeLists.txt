cmake_minimum_required(VERSION 2.4)
project(stbimage)

file(GLOB_RECURSE SRCS *.cpp *.h)
add_library(stbimage ${SRCS})
