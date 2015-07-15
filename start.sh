#!/bin/bash
g++ Menu.cpp -lGL -lGLU -lglut -lSOIL -o TRONSTART;
g++ Tron.cpp -lGL -lGLU -lglut -lSDL -lSDL_mixer -o TRON;
./TRONSTART;
