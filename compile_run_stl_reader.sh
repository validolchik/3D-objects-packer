#!/bin/sh
echo "Compiling"
exec g++ -o reader ascii_stl_reader.cpp
echo "Compiled"
exec ./reader models/foot_ascii.stl
