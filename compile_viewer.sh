#!/bin/sh
echo "compiling opengl_viewer.."
g++ -o viewer opengl_viewer.cpp -lglut -lGLU -lGL
ret_code=$?
echo "viewer compiled, return code $ret_code"
exit $ret_code