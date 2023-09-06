cmake_minimum_required(VERSION 3.24)
project(Asteroids_Game C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Asteroids_Game
        asteroids.c
        asteroids.h
        move_ship.c)
