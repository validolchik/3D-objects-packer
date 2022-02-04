#!/bin/sh
g++ -o viewer opengl_viewer.cpp -lglut -lGLU -lGL
ret_code=$?
echo "viewer compiled"
exit $ret_code