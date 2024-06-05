#!/bin/bash

g++ -std=c++17 -o Train src/Main.cpp src/OpenGL.cpp src/textures/textures.cpp src/OpenAL/OpenAL.cpp -framework OpenGL -framework GLUT -framework OpenAL -lsndfile

if [ $? -eq 0 ]; then
    echo "success"
    ./Train
else
    echo "failed"
fi
