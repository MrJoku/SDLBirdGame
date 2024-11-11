#!/bin/bash
g++ -o main main.cpp `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf
