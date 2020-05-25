#!/bin/sh
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .

cmake --build .

Xvfb :2 -screen 0 800x600x24+32 & fluxbox & x11vnc -forever -repeat
