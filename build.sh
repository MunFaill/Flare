#!/bin/bash

echo "This is a simple build script for quickly setting up the project and running tests;
for production use, it is recommended to create your own script or perform a manual build."

rm -fr build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build/ -j$(nproc)
