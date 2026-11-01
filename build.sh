#!/bin/bash
set -e  # stop on errors

echo "Starting build..."

cd ./fun
unzip -o asio.zip -d asio

ls -R
# Compile your C++ backend
# Remove Windows-specific libs (-lws2_32 -lmswsock)
g++ -g -Wall -I. backend.cpp -o backend -pthread

echo "Build complete."