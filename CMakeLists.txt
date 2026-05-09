cmake_minimum_required(VERSION 4.1)
project(attempt2 C)

set(CMAKE_C_STANDARD 99)

add_executable(attempt2 main.c
        waveform.c
        io.c
        waveform.h)
