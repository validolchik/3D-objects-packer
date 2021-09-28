#!/bin/sh
echo "Compiling"
exec g++ -o viewer_all stl_viewer.cpp -lglut -lGLU -lGL
echo "Compiled"