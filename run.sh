#!/bin/bash

g++ -std=c++17 -o Train Main.cpp OpenGL.cpp textures/textures.cpp OpenAL/OpenAL.cpp -framework OpenGL -framework GLUT -framework OpenAL -lsndfile

if [ $? -eq 0 ]; then
    echo "success"
    ./Train
else
    echo "failed"
fi
