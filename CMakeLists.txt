cmake_minimum_required(VERSION 3.28)
project(CS211 C)

set(CMAKE_C_STANDARD 11)

add_executable(CS211
        Project01/main.c
        Project03/test.c
        Project03/MinPopVote.c
        Project03/MinPopVote.h
        Project03/app.c)
