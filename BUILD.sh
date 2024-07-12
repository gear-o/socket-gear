#!/bin/sh

cmake ./ -B./build
cd ./build
make all
# mv cmakeTest ../