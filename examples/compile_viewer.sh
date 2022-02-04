#!/bin/sh
echo "Compiling"
exec g++ -o viewer opengl_viewer.cpp -lglut -lGLU -lGL
echo "Compiled"
