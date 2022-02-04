#!/bin/sh
echo "Compiling"
g++ -o viewer_all stl_viewer.cpp -lglut -lGLU -lGL
echo "Compiled to file viewer_all"